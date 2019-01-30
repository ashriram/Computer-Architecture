#include <stdlib.h>
#include <stdio.h>
#include "heaplib.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#define ADD_BYTES(base_addr, num_bytes) (((char *)(base_addr)) + (num_bytes))
#define BYTE_ALIGNED(addr) (addr % 8 ==0)


typedef struct _block_header_t {
    unsigned int block_size; //will include the header size
    unsigned int allocated; //0 = free, 1 is used
    void* next_block; //next block is always greater than current
    void* prev_block; //prev block is always less than current
} block_header_t;

typedef struct _heap_header_t {
  //heap_header_size and heap_header_size may not be needed but I think we should keep it for now
    unsigned int heap_size;
    unsigned int heap_header_size;
    unsigned int header_size; //block header size
    block_header_t* first_block; //pointer to first free block
} heap_header_t ;



/* -------------------- PRINT DEBUG FNS ----------------- */

void print_debug_heap_header(heap_header_t *header) {
#ifdef PRINT_DEBUG
    printf("heap starts at addr %p\n"   // C printing trick.
           "heap_size = %u\n"
           "first_block = %p\n",            // Notice: no commas between lines
           header, header->heap_size,header->first_block);
  #endif
}

void print_block(block_header_t *b) {
#ifdef PRINT_DEBUG
printf("START BLOCK INFO\n");
    printf("block addr %p\n"
           "block_size = %u\n"
           "allocated = %u\n"
           "next: %p\n"
           "prev: %p\n",
           b,b->block_size, b->allocated,b->next_block,b->prev_block);
           printf("END BLOCK INFO\n");
  #endif
}

void print_debug_sizes(heap_header_t *header) {
#ifdef PRINT_DEBUG
    printf("heap header size %u\n"   // C printing trick.
           "block header size = %u\n",
           header->heap_header_size,header->header_size);
  #endif
}

void print(char* to_print) {
#ifdef PRINT_DEBUG
    printf("String %s\n",to_print);
  #endif
}

void print_int(int a){
  #ifdef PRINT_DEBUG
      printf("Integer %u\n",a);
    #endif
}

void print_p(void* a){
  #ifdef PRINT_DEBUG
      printf("Pointer %p\n",a);
    #endif
}

void start_alloc(){
  #ifdef PRINT_DEBUG
      // printf("Starting Alloc\n");
    #endif
}

void end_alloc(){
  #ifdef PRINT_DEBUG
      // printf("Ending Alloc\n");
    #endif
}


void print_free_blocks(heap_header_t *header) {
#ifdef PRINT_DEBUG
    block_header_t* block = header->first_block;
    int i = 0;
    printf("FREE BLOCKS START\n");
    while(block != NULL)
    {
      printf("block %u\n"
             "block address %p\n"
             "next block address %p\n"
             "block size = %u\n",
             i,block,block->next_block, block->block_size);
      block = block->next_block;
      i = i+1;
    }
    printf("FREE BLOCKS END\n");

  #endif
}
/* -------------------- END DEBUG FNS ----------------- */



/*
 * Rounds any integer the nearest (higher) integer divisible by 8
 */
int round_to_8(int a){
    int p = a % 8;
    if(p == 0){
      return a;
    }
    else{
      return (a + (8-p));
    }
}

/*
 * Rounds any integer the nearest (higher) integer divisible by 8
 */
heap_header_t* round_header(void* a){
    uintptr_t p = ((uintptr_t)a) % 8;
    if(p == 0){
      return a;
    }
    else{
      return (heap_header_t*) ADD_BYTES(a,(8-p));
    }
}
// void set_block(void* block_addr, int used, )
/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void hl_init(void *heap, unsigned int heap_size) {
  //the heap needs to initial the heap_header_t
  // additionally the free block needs to be initalized and have null next and prev
  heap_header_t* heap_start = round_header(heap);
  heap_start->heap_size = heap_size;
  heap_start->header_size = round_to_8(2 * (sizeof(unsigned int) + sizeof(void*)));
  heap_start->heap_header_size = round_to_8(3*sizeof(unsigned int) + heap_start->header_size);

  block_header_t* first_block = (block_header_t*) ADD_BYTES((void*) heap_start,heap_start->heap_header_size);
  first_block->block_size = heap_size - (heap_start->heap_header_size); //space that is left
  first_block->prev_block = NULL;
  first_block->next_block = NULL;
  first_block->allocated = 0;

  heap_start->first_block = first_block;
  print_debug_heap_header(heap_start);
  print_debug_sizes(heap_start);
}
/*
 * Finds the first block that fits the size needed and returns its header
 * when the method works, ideally need to find a block that is the closest to size
 */
void* matching_block(block_header_t* starting_block, int size){
  block_header_t* traversal_pointer = starting_block;
  print("match 1");
  printf("%p\n", traversal_pointer);
  // print_block(traversal_pointer);
  int min_dif = 2147483647;
  block_header_t* returning = traversal_pointer;
  print("match 2");

  // print_p(traversal_pointer);
  while(traversal_pointer !=NULL)
  {
    print("match 3");

    int temp = traversal_pointer->block_size - size;
    if(temp == 0){ return traversal_pointer;}
    if(temp > 0 && temp < min_dif)
    {
      print("match 3.2");
      returning = traversal_pointer;
    }
    print("match 3.3");
    print_p(traversal_pointer->next_block);
    traversal_pointer = traversal_pointer->next_block;
  }
  if(returning != NULL)
  {
    print("match 3.4");
    print_int(returning->block_size);
    print_int(size);

    if (returning->block_size >= size)
    {
      print("match 3.6");
      return returning;
    }
    else
    {
      print("match 3.8");

      return NULL;
    }
  }
  else{
    print("match 4");
    return NULL;
  }
}
/*
 * Combines free adjacent blocks to blocks of a larger size
 */
void combine_blocks(heap_header_t* heap)
{
  block_header_t* traversal_pointer = heap->first_block;
  while(traversal_pointer->next_block != NULL){
    void* v = ADD_BYTES(traversal_pointer, traversal_pointer->block_size);
    if(0 == (v - traversal_pointer->next_block))
    {
      traversal_pointer->block_size = traversal_pointer->block_size + ((block_header_t*)(traversal_pointer->next_block))->block_size;
      traversal_pointer->next_block = ((block_header_t*)(traversal_pointer->next_block))->next_block;
    }
    else
    {
      traversal_pointer = traversal_pointer->next_block;
    }
  }
}
/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void *hl_alloc(void *heap, unsigned int block_size) {
  if(block_size == 0){return NULL;}
  start_alloc();
  heap_header_t* heap_start = round_header(heap);
  // print("pos1");
  if(block_size > heap_start->heap_size) {return NULL;}
  // print_free_blocks(heap_start);
  int total_block_size = round_to_8(block_size + heap_start->header_size);
  print("pos2");
  block_header_t* next_block = (block_header_t*) matching_block(heap_start->first_block, total_block_size);
  if(next_block == NULL){
    print("pos2.5");
    return NULL;
  }
  print("pos3");
  next_block->allocated = 1;
  int free_space_after = 0;
  free_space_after = (void*) (ADD_BYTES(next_block,next_block->block_size)) - (void*) (ADD_BYTES(next_block,total_block_size));
  if(free_space_after >= heap_start->header_size)
  {
    //allocate the new block
    print("pos4");
    block_header_t* new_free_block = (block_header_t*) ADD_BYTES(next_block,total_block_size);
    new_free_block->allocated = 0;
    new_free_block->prev_block = next_block->prev_block;
    new_free_block->next_block = next_block->next_block;
    //this new block replaces the old free block, so takes it's place in the list
    new_free_block->block_size = next_block->block_size - total_block_size;
    //^ difference between original free space and newly allocated block
    if(next_block->prev_block == NULL)
    {
      print("IS THIS IT?");
      print_block(new_free_block);
      print_debug_sizes(heap_start);
      // print_p(new_free_block)
      heap_start->first_block = new_free_block;
      //if prev block is null then it was the first block
    }
  }
  else
  {
    print("pos5");
    if(next_block->prev_block != NULL)
    {
      print("pos6");
      ((block_header_t*)(next_block->prev_block))->next_block = next_block->next_block;
      if(next_block->next_block != NULL)
      {
        ((block_header_t*)(next_block->next_block))->prev_block = next_block->prev_block;
      }
    }
    else
    {
      print("pos7");
      heap_start->first_block = next_block->next_block;
      //if prev is null it is the first block, and its sucessor is the first block
    }
  }
  next_block->block_size = total_block_size; //may not be needed, i have used it tho
  void* p  = ADD_BYTES(next_block,heap_start->header_size);
  print("FIRST BLOCK OF HEADER");
  print_p(heap_start->first_block);
  end_alloc();
  return p;
  // ^need to add the header size to the return

  }

/* Finds the correct position for block in heap so that address of prev_block < current block < next_block
 * is maintained.
 *
 */
block_header_t* find_position(heap_header_t* heap, void* block)
{

  //need to check if the address go in correct order (do the addresses go from lowest to highest)
  block_header_t* traversal_pointer = heap->first_block;
  int prev = (void*)traversal_pointer - block;
  if (prev > 0)
  {
    //means that block should be the first in the block list
    return NULL;
  }
  while(traversal_pointer!=NULL)
  {
    int forward = (void*) traversal_pointer->next_block - block;
    //first time when block forward is ahead of the block
    if(forward >= 0)
    {
      return traversal_pointer;
    }
    traversal_pointer =  traversal_pointer->next_block;
  }
  return traversal_pointer;
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void hl_release(void *heap, void *block) {
  if (block != NULL)
  {
    heap_header_t* heap_start = round_header(heap);
    int block_offset = (-1*heap_start->header_size);
    block_header_t* block_start = (block_header_t*) ADD_BYTES(block,block_offset);
    block_start->allocated = 0; //freed + block size stays the same
    print_p(block);
    print("release");
    block_header_t* previous_block = find_position(heap_start, block_start);
    // print_p(previous_block);
    if ((void*) previous_block == NULL)
    {
          //newly free block should be first
      // print_block(block_start);
      print("yea");
      block_start->next_block = heap_start->first_block;
      block_start->prev_block = NULL;
      heap_start->first_block = (void*) block_start;

    }
    else
    {
      print("no");
      block_start->next_block = previous_block->next_block;
      block_start->prev_block = (void*) previous_block;
    }
    combine_blocks(heap_start);
  }
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void *hl_resize(void *heap, void *block, unsigned int new_size) {
    if(block == NULL)
    {
      return hl_alloc(heap,new_size);
    }
    if(new_size == 0)
    {
      hl_release(heap,block);
      return NULL;
    }
    block_header_t* new_block = hl_alloc(heap,new_size);
    if (new_block == NULL){return NULL;}
    heap_header_t* heap_start = round_header(heap);
    int block_offset = (-1*heap_start->header_size);
    block_header_t* block_start = (block_header_t*) ADD_BYTES(block,block_offset);
    int b_size = block_start->block_size;
    if((b_size - heap_start->header_size) == new_size)
    {
      return block;
    }
    if(b_size == 0)
    {
      return hl_alloc(heap,new_size);
    }
    if (new_size < (b_size - heap_start->header_size))
    {
      memmove(new_block,block,new_size);
    }
    else
    {
      memmove(new_block,block,(b_size - heap_start->header_size));
    }
    hl_release(heap, block);
    return new_block;
}
-
