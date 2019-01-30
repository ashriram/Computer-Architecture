//resize -->memory not copied
//resize and free
// global variables -- > N_SUPPORTED_BLOCKS + global structs
//allocatin g a heap
//hl alloc can return blocks of size 0 == NOP
//heap size allocations


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "heaplib.h"

#define HEAP_SIZE 1024
#define NUM_TESTS 24
#define NPOINTERS 100

#define ADD_BYTES(base_addr, num_bytes) (((char *)(base_addr)) + (num_bytes))


// TODO: Add test descriptions as you add more tests...
const char* test_descriptions[] = {
    /* our SPEC tests */
    /* 0 */ "single init, should return without error",
    /* 1 */ "single init then single alloc, should pass",
    /* 2 */ "single alloc which should fail b/c heap is not big enough",
    /* 3 */ "multiple allocs, verifying no hard-coded block limit",
    /* your SPEC tests */
    /* 4  */ "multiple allocs, checking alloc and resize they are 8byte aligned",
    /* 5  */ "tests whether you get alloc of size and null resize = alloc",
    /* 6  */ "release if block is null is a NOP",
    /* 7  */ "alloc size 0, release block_size 0, resize of block_size 0, resize size reduction",
    /* 8  */ "data is not overwritten when resize is called",
    /* 9  */ "released memory can be reallocated",
    /* 10 */ "null upon impossible resize or alloc",
    /* 11 */ "no test",
    /* 12 */ "no test",
    /* 13 */ "no test",
    /* 14 */ "no test",
    /* 15 */ "no test",
    /* STRESS tests */
    /* 16 */ "alloc & free, stay within heap limits, testing whether whole block is availible",
    /* 17 */ "alloc & free, stay within heap limits, randomly filling the blocks",
    /* 18 */ "alloc & free, stay within heap limits, with random block sizes",
    /* 19 */ "alloc & free, stay within heap limits, with random block sizes and checking data integrity of each block",
    /* 20 */ "allocate and fill blocks, then free and resize, while checking data integrity",
    /* 21 */ "data is not overwritten when resized",
    /* 22 */ "allocate and fill blocks, then 90% resize, 10% release, while checking data integrity",
    /* 23 */ "allocating maximum space, then releasing and allocating"
};
void print_t(char* to_print) {
#ifdef PRINT_DEBUG
    printf("Test: %s\n",to_print);
  #endif
}
void print_int2(int a){
  #ifdef PRINT_DEBUG
      printf("Integer %d\n",a);
    #endif
}

typedef struct _block_header_t {
    unsigned int block_size; //will include the header size
    unsigned int allocated; //0 = free, 1 is used
    void* next_block; //next block is always greater than current
    void* prev_block; //prev block is always less than current
} block_header_t;

void print_block2(block_header_t *b) {
#ifdef PRINT_DEBUG
printf("START BLOCK INFO\n");
    printf("block addr %p\n"
           "block_size = %d\n"
           "allocated = %d\n"
           "next: %p\n"
           "prev: %p\n",
           b,b->block_size, b->allocated,b->next_block,b->prev_block);
           printf("END BLOCK INFO\n");
  #endif
}

/* ------------------ COMPLETED SPEC TESTS ------------------------- */

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init
 * SPECIFICATION BEING TESTED:
 * hl_init should successfully complete (without producing a seg
 * fault) for a HEAP_SIZE of 1024 or more.
 *
 * MANIFESTATION OF ERROR:
 * A basic test of hl_init.  If hl_init has an eggregious programming
 * error, this simple call would detect the problem for you by
 * crashing.
 *
 * Note: this shows you how to create a test that should succeed.
 */
int test00() {

    // simulated heap is just a big array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    return SUCCESS;
}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * If there is room in the heap for a request for a block, hl_alloc
 * should sucessfully return a pointer to the requested block.
 *
 * MANIFESTATION OF ERROR:
 * The call to hl_alloc will return NULL if the library cannot find a
 * block for the user (even though the test is set up such that the
 * library should be able to succeed).
 */
int test01() {

    // simulated heap is just a big array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    // if this returns null, test01 returns FAILURE (==0)
    return (hl_alloc(heap, HEAP_SIZE/2) != NULL);
}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * If there is not enough room in the heap for a request for a block,
 * hl_alloc should return NULL.
 *
 * MANIFESTATION OF ERROR:
 * This test is designed to request a block that is definitely too big
 * for the library to find. If hl_alloc returns a pointer, the library is flawed.
 *
 * Notice that heaplame's hl_alloc does NOT return NULL. (This is one
 * of many bugs in heaplame.)
 *
 * Note: this shows you how to create a test that should fail.
 *
 * Surely it would be a good idea to test this SPEC with more than
 * just 1 call to alloc, no?
 */
int test02() {

    // simulated heap is just an array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    // if this returns NULL, test02 returns SUCCESS (==1)
    return !hl_alloc(heap, HEAP_SIZE*2);

}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * There should be no hard-coded limit to the number of blocks heaplib
 * can support. So if the heap gets larger, so should the number of
 * supported blocks.
 *
 * MANIFESTATION OF ERROR:
 * See how many blocks are supported with heap size N. This number should
 * increase with heap size 2N. Otherwise fail!
 *
 * Note: unless it is fundamentally changed, heaplame will always fail
 * this test. That is fine. The test will have more meaning when run on
 * your heaplib.c
 */

int test03() {

    // now we simulate 2 heaps, once 2x size of the first
    char heap[HEAP_SIZE], heap2[HEAP_SIZE*2];
    int num_blocks = 0;
    int num_blocks2 = 0;

    hl_init(heap, HEAP_SIZE);

    while(true) {
        int *array = hl_alloc(heap, 8);
        if (array)
            num_blocks++;
        else
            break;
    }

    hl_init(heap2, HEAP_SIZE*2);

    while(true) {
        int *array = hl_alloc(heap2, 8);
        if (array)
            num_blocks2++;
        else
            break;
    }
    // num_blocks2 = 5;
#ifdef PRINT_DEBUG
    printf("%d blocks (n), then %d blocks (2n) allocated\n", num_blocks, num_blocks2);
#endif

    // hoping to return SUCCESS (==1)
    return (num_blocks2 > num_blocks);
}

/* ------------------ YOUR SPEC TESTS ------------------------- */

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: alloc, resize
 * SPECIFICATION BEING TESTED:
 * The pointers returned for alloc and resize should be 8-byte aligned
 * even if block size is not 8-byte aligned
 * and enen if heap is not 8-byte aligned.
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test04() {

    for(int i = 0; i< 200; i++)
    {
      char heap[HEAP_SIZE];
      hl_init(heap, HEAP_SIZE);
      void* test;
      int block_size = 5; //to keep it from being 8 byte aligned
      test = hl_alloc(heap, block_size);
      uintptr_t t =(uintptr_t) test;
      if (t%8 != 0)
      {
        return FAILURE;
      }
      test = hl_resize(heap, test, 9);
      uintptr_t t1 =(uintptr_t) test;
      if (t1%8 != 0)
      {
        return FAILURE;
      }
      void* heap2 = malloc(HEAP_SIZE * 2);
      void* old = heap2;
      uintptr_t p = ((uintptr_t)heap2)%8;
      heap2 = ADD_BYTES(heap2,(8-p));
      heap2 = ADD_BYTES(heap2, 3);
      hl_init(heap2, HEAP_SIZE);
      block_size = 8;
      test = hl_alloc(heap2, block_size);
      t =(uintptr_t) test;
      if (t%8 != 0)
      {
        return FAILURE;
      }
      test = hl_resize(heap2, test, 9);
      t1 =(uintptr_t) test;
      if (t1%8 != 0)
      {
        return FAILURE;
      }
      free(old);
  }
  return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: alloc, resize
 * SPECIFICATION BEING TESTED:
 * checking if a block of size is returned with alloc
 * resize with a NULL block is identical to alloc
 *
 * MANIFESTATION OF ERROR:
 * segfault is the appropriate memory is not allocated
 * number of blocks with a null resize of 2*block_size < alloc of block size
 */

int test05(){
  for(int p = 0; p < 250; p++)
  {
    char heap[HEAP_SIZE];
    char heap2[HEAP_SIZE];
    int num_blocks = 0;
    int num_blocks2 = 0;
    int block_size = 8;
    hl_init(heap, HEAP_SIZE);
    char* block = (char*) hl_alloc(heap,block_size);
    while (block!=NULL)
    {
        block = (char*) hl_alloc(heap,block_size);
        if(block !=NULL)
        {
          for(int i = 0; i < block_size; i++)
          {
            block[i] = 'a';//shouldn't segfault
          }
        }

        num_blocks++;
    }
    print_t("normal alloc");
    hl_init(heap2, HEAP_SIZE);
    block = (char*) hl_resize(heap2,NULL,2*block_size);
    while (block!=NULL)
    {
        block = (char*) hl_resize(heap2,NULL,2*block_size); // behaves like alloc
        if(block !=NULL)
        {
          for(int i = 0; i < 2*block_size; i++)
          {
            block[i] = 'b';//shouldn't segfault if allocated
          }
        }

        else{print_t("yes");}
        num_blocks2++;
    }

    if (num_blocks2 > num_blocks)
    {
      return FAILURE;
    };
  }
  return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: release
 * SPECIFICATION BEING TESTED:
 * release if block is null is a NOP
 *
 * MANIFESTATION OF ERROR:
 * the number of blocks will be different
 */
int test06(){

  for(int j = 0 ; j < 250; j++)
  {
      char heap[HEAP_SIZE];
      char heap2[HEAP_SIZE];
      char heap3[HEAP_SIZE];
      int block_size = 8;
      print_t("yes2");

      hl_init(heap, HEAP_SIZE);
      int block1 = 0;
      int block2 = 0;
      int k = 0;

      void* block = hl_alloc(heap,block_size);
      while (block!=NULL)
      {
        block = hl_alloc(heap,block_size);
        block1= block1+1;
      }
      hl_init(heap2, HEAP_SIZE);
      block = hl_alloc(heap2,block_size);
      while (block!=NULL)
      {
        block = hl_alloc(heap2,block_size);
        hl_release(heap2, NULL);
        block2= block2+1;
      }
      if (block2 != block1)
      {
        return FAILURE;
      }
      //release should not be a nop if used properly
      hl_init(heap3, HEAP_SIZE);
      block = hl_alloc(heap3,block_size);
      while (block!=NULL && k < 1000)
      {
        block = hl_alloc(heap3,block_size);
        hl_release(heap3, block);
        k = k+1;
      }
      if (k <1000){return FAILURE;}
  }

    return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: alloc
 * SPECIFICATION BEING TESTED:
 * allows for a block to be allocated of size 0
 * release works if block size is 0
 * resize does not fail alloc if size is 0
 * resize works (contents are unchanged) with size less than original block size
 *
 *
 * MANIFESTATION OF ERROR:
 * will fail if trying to resize or release a block of size 0 if alloc doesn't treat
 * alloc size 0 properly
 */

int test07() {
  for (int j = 0; j< 1000;j++)
  {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    void* block;
    char* array;
    //allocate block of size 0
    block = hl_alloc(heap, 0); //should segfault or some issue if 0 size fails
    void* b1 = hl_alloc(heap, 7); //check if heap still works
    if(b1 == NULL){return FAILURE;}

    //resize to a larger size
    block = hl_resize(heap, block, 16); //should be able to sucessfully resize to size 16
    if(block==NULL){return FAILURE;}
    b1 = hl_alloc(heap, 12);
    array = (char*) block;
    //filling the block with some value
    for(int i = 0 ; i < 16; i++)
    {
      array[i] = 'c'; // should not segfault
    }
    //resize back to size 0, shouldn't cause segfault or other issue
    block = hl_resize(heap, block, 0);
    b1 = hl_alloc(heap, 15);
    block = hl_alloc(heap,0);

    // free block of size 0
    hl_release(heap, block);
    block = hl_resize(heap, NULL,0);
    b1 = hl_alloc(heap, 15);
    hl_release(heap,b1);
    void* block2 = hl_alloc(heap,32);
    if(block2==NULL){return FAILURE; }

    array = (char*) block2;
    for(int i = 0; i< 32; i++)
    {
      array[i] = 'y';
    }
    block2 = hl_resize(heap,block2,16);
    if(block2==NULL){return FAILURE;}
    array = (char*) block2;
    for(int i = 0; i< 16; i++)
    {
      if(array[i] != 'y')
      {
        return FAILURE;
      }
    }
  }
  return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: resize
 * SPECIFICATION BEING TESTED:
 * data is not overwritten when resize is called
 *
 * MANIFESTATION OF ERROR:
 * If incorrect, data will be overwritten when resize is called again.
 */
int test08() {
  for (int j = 0; j< 1000;j++)
  {
      char heap[HEAP_SIZE*8];
      int block_size = 0;
      if(j%3 == 0){ block_size = 16;}
      else if(j%3==1){block_size = 23;}
      else {block_size = 43;} //43,45,46,48,54
      hl_init(heap, HEAP_SIZE*8);
      char* block_0 = hl_alloc(heap, block_size);
      char* block_2 = hl_alloc(heap, block_size);

      //filling block_0 with p
      for(int i = 0 ; i < block_size; i++)
      {
        block_0[i] = 'p';
        block_2[i] = 't';
      }
      char* block_1 = hl_alloc(heap, block_size*6);
      char* block = hl_alloc(heap, block_size);
      while(block!=NULL)
      {
        for(int i = 0 ; i< block_size; i++)
        {
          block[i] = 'r';
        }
        //printf("filling another block");
        block = hl_alloc(heap, block_size);
      }
      hl_release(heap,block_1); //freeing enough space for hypothetical issues
      block_0 = hl_resize(heap,block_0, (1.5*block_size)); //resizing block
      if(block_0 == NULL){
        return FAILURE;
        }//should be able to resize

      for(int k= 0; k < 1.5*block_size;k++)
      {
          if (k < block_size && block_0[k] != 'p') //no segfaults
          {
            return FAILURE;
          }
      }
      block_0 = hl_resize(heap,block_0, (0.5*block_size));

      if(block_0 == NULL){
        return FAILURE;
        }
      for(int k= 0; k < (int)(0.5*block_size);k++)
      {
          if (block_0[k] != 'p'){
            return FAILURE;
            }
      }
      block_2 = hl_resize(heap,block_2,(block_size));//resizing block to same size
      for(int k= 0; k < block_size;k++)
      {
          if (block_2[k] != 't'){
            return FAILURE;
            }
      }
      block_2 = hl_resize(heap,block_2,(0.75*block_size));//resizing block to same size
      if(block_2 != NULL)
      {
        for(int k= 0; k < (int)(0.75*block_size); k++)
        {
            if (block_2[k] != 't'){
              return FAILURE;
              }
        }
      }
  }
  return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: release
 * SPECIFICATION BEING TESTED:
 * released block can be allocated again
 *
 * MANIFESTATION OF ERROR:
 * fill everything, free one 8B block, then try to alloc one 8B block
 * if it fails, then it's broken
 */
int test09() {
  char heap[HEAP_SIZE];
  hl_init(heap, HEAP_SIZE);
  char *pointers[5];
  memset(pointers, 0, 5*sizeof(char *));
  for (int k = 0; k < 1000; k++)
  {
    if(pointers[k%5]==0)
    {
      pointers[k%5] = hl_alloc(heap,16);
      if(pointers[k%5] != NULL)
      {
        char* arr = pointers[k%5];
        for(int t = 0; t < 16; t++)
        {
          arr[t] = 'a';
        }
      }
      else
      {
        print_t("test9");
        return FAILURE;
      }
    }
    else
    {
      printf("DD %p\n",pointers[k]);
      print_t("llllllllllllllllllllllllllllllllllllllllllllllllllllllll");
      hl_release(heap,pointers[k%5]);
      pointers[k%5] = 0;
    }
  }
  return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: resize, alloc
 * SPECIFICATION BEING TESTED:
 * returns 0 if resize request cannot be satisfied
 * returns 0 if alloc request cannot be satisfied

 * MANIFESTATION OF ERROR:
 * There is an error if the heap is full and you try to increase
 * the size of one of the blocks
 * There is an error if the heap is full and you try to alloc again
 */
int test10() {
  for (int j = 0; j< 1000;j++)
  {
      char heap[HEAP_SIZE];
      int block_size=0;
      if(j%3 == 0){ block_size = 16;}
      else if(j%3==1){block_size = 23;}
      else{block_size = 64;}
      hl_init(heap, HEAP_SIZE);
      void* block = hl_alloc(heap, block_size);
      //allocate all memory
      void* b1 = hl_alloc(heap,block_size);
      while(block != NULL)
      {
          block = hl_alloc(heap, block_size);
      }
      //try to resize a block to be bigger
      void* resized = hl_resize(heap, b1, block_size*4);
      if(resized != NULL)
      { //indicates that the block was (incorrectly) resized
          return FAILURE;
      }
      //try to allocate more memory
      void* alloc = hl_alloc(heap, block_size*4);
      if(alloc != NULL){ //indicates that the block (correctly) cannot be allocated
          return FAILURE;
      }
  }
  return SUCCESS;
}

int test11()
{
  for (int j = 0; j< 1000;j++)
  {
      char heap[HEAP_SIZE];
      hl_init(heap, HEAP_SIZE);
      int block_size = 0;
      if(j%3 == 0){ block_size = 16;}
      else if(j%3==1){block_size = 23;}
      else{block_size = 64;}
      void* block = hl_alloc(heap, block_size);
      void* b1 = hl_alloc(heap,block_size);
      //allocate all memory
      while(block != NULL){
          b1 = block;
          block = hl_alloc(heap, block_size);
      }
      //free one block
      hl_release(heap, b1);
      //try to allocate again, should have one space open
      //will not fail gracefully if broken
      block = hl_alloc(heap, block_size); // should allow one more alloc
      if(block ==NULL){
        return FAILURE;
      }
      char* array = (char*) block;
      for(int t = 0; t< block_size; t++)
      {
        array[t] = 'k';//shouldn't segfault
      }
      block = hl_alloc(heap,block_size);

      if(block !=NULL){return FAILURE;}
  }
  return SUCCESS;
}

int test12() {
  return SUCCESS;
}
int test13()
{
    return SUCCESS;
}
int test14()
{
    return SUCCESS;
}
int test15()
{
    return SUCCESS;
}

/* ------------------ STRESS TESTS ------------------------- */

/* THIS TEST IS NOT FINISHED. It is a stress test, but it does not
 * actually test to see whether the library or the user writes
 * past the heap. You are encouraged to complete this test.
 *
 * FUNCTIONS BEING TESTED: alloc, free
 * SPECIFICATION BEING TESTED:
 * The library should not give user "permission" to write off the end
 * of the heap. Nor should the library ever write off the end of the heap.
 *
 * MANIFESTATION OF ERROR:
 * If we track the data on each end of the heap, it should never be
 * written to by the library or a good user.
 *
 */
int test16() {
    int n_tries    = 10000;
    int block_size = 16;
    char memarea[HEAP_SIZE*3];
    char *heap_start = &memarea[HEAP_SIZE];// heap will start 1024 bytes in
    char *pointers[NPOINTERS];
    memset(pointers, 0, NPOINTERS*sizeof(char *));
    memset(memarea, 5, HEAP_SIZE*3);
    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        if (pointers[index] == NULL)
        {
            pointers[index] = hl_alloc(heap_start, block_size);
            char* array = (char*) pointers[index];
            uintptr_t q1 = (uintptr_t) array;
            if(q1%8 !=0){return FAILURE;}
            if(pointers[index] != NULL)
            {
              for(int t = 0 ; t < block_size;t++)
              {
                array[t] = '@';
              }
            }
        }
        else{
            char* array = (char*) pointers[index];
            for(int t = 0 ; t < block_size;t++)
            {
              if (array[t] != '@'){return FAILURE;}
            }
            hl_release(heap_start, pointers[index]);
            pointers[index] = NULL;
        }
        // int p  = 2*HEAP_SIZE + 1;
        for(int p  = 2*HEAP_SIZE; p < (3*HEAP_SIZE - 1);p++)
        {
            if (memarea[p] != 5) {return FAILURE;}
        }
    }
    return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test17() {
    int n_tries    = 10000;
    int block_size = 16;
    char memarea[HEAP_SIZE*3];
    for (int i = 0; i < 3*HEAP_SIZE; i++)
    {
      memarea[i] = '^';
    }
    char *heap_start = &memarea[HEAP_SIZE]; // heap will start 1024 bytes in
    char *pointers[NPOINTERS];
    memset(pointers, 0, NPOINTERS*sizeof(char *));
    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++)
    {
        int index = random() % NPOINTERS;
        if (pointers[index] == NULL)
        {
            pointers[index] = hl_alloc(heap_start, block_size);
            if(pointers[index] != NULL)
            {
              char* array = (char*) pointers[index];
              uintptr_t q1 = (uintptr_t) array;
              if(q1%8 !=0){return FAILURE;}
              for(int j = 0; j < block_size; j++)
              {
                int t =  random();
                if(t%2 == 0)
                {
                  array[j] = 'b';
                }
              }
            }
        }
        else
        {
          hl_release(heap_start, pointers[index]);
          pointers[index] = NULL;
        }
    }
    for (int i = 0; i < HEAP_SIZE; i++)
    {
      if (memarea[i] != '^')
      {
        return FAILURE;
      }
    }
    for(int i = 2*HEAP_SIZE; i < 3*HEAP_SIZE; i++)
    {
      if (memarea[i] != '^')
      {
        return FAILURE;
      }
    }
    return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: alloc
 * INTEGRITY OR DATA CORRUPTION?
 * randomize block size
 * MANIFESTATION OF ERROR:
 *
 */
int test18() {
    int n_tries = 5000;
    int block_size;
    char memarea[HEAP_SIZE*3];
    //fill padding with an arbitrary value
    for (int i = 0; i < 3*HEAP_SIZE; i++)
    {
      memarea[i] = 'a';
    }
    char *heap_start = &memarea[HEAP_SIZE]; // heap will start 1024 bytes in
    char *pointers[NPOINTERS];
    memset(pointers, 0, NPOINTERS*sizeof(char *));
    int block_options[] = {1,5,8,14,23,32,64,71,88,101,223};
    int num_blocks = 11;
    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        block_size = block_options[random() % num_blocks];
        if (pointers[index] == NULL)
        {
            pointers[index] = hl_alloc(heap_start,  block_size);
            if(pointers[index]!= NULL)
            {
              char* array  = (char*) pointers[index];
              uintptr_t q1 = (uintptr_t) array;
              if(q1%8 !=0){return FAILURE;}
              for(int j = 0; j < block_size; j++)
              {
                array[j] = 'b';
              }
            }
        }
        else
        {
            hl_release(heap_start, pointers[index]);
            pointers[index] = NULL;
        }
    }

    for (int i = 0; i < HEAP_SIZE; i++)
    {
      if (memarea[i] != 'a'){
        return FAILURE;
      }
    }
    for(int i = 2*HEAP_SIZE; i < 3*HEAP_SIZE; i++)
    {
      if (memarea[i] != 'a'){
        return FAILURE;
      }
    }

    return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 * randomize block size and use distinct values in each block
 * MANIFESTATION OF ERROR:
 *
 */
int test19() {
    int n_tries = 10000;
    int block_size;
    char memarea[HEAP_SIZE*3];
    //fill padding with an arbitrary value
    for (int i = 0; i < 3*HEAP_SIZE; i++)
    {
      memarea[i] = '!';
    }
    char *heap_start = &memarea[HEAP_SIZE]; // heap will start 1024 bytes in
    char *pointers[NPOINTERS];
    int block_options[] = {1,5,8,14,23,32,64,71,88,101,223};
    int num_blocks = 11;
    int block_sizes[NPOINTERS];
    char characters[26];
    for (int i = 0; i < 26;i++)
    {
      characters[i] = i+'a';
    }
    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));

    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++)
    {
        int index = random() % NPOINTERS;
        block_size = block_options[random() % num_blocks];
        if (pointers[index] == NULL)
        {
            pointers[index] = hl_alloc(heap_start, block_size);
            if(pointers[index] != NULL)
            {
              char* array = (char*) pointers[index];
              uintptr_t q1 = (uintptr_t) array;
              if(q1%8 !=0){return FAILURE;}
              for(int j = 0; j < block_size; j++)
              {
                array[j] = characters[index%26];
              }
              block_sizes[index] = block_size;
            }
        }
        else
        {
            hl_release(heap_start, pointers[index]);
            pointers[index] = NULL;
        }
        for(int j = 0; j < NPOINTERS; j++)
        {
          for(int b = 0; b < block_sizes[j]; b++)
          {
            char* array = (char*) pointers[j];
            if(array != NULL)
            {
              if(array[b] != (characters[j%26]))
              {
                return FAILURE;
              }
            }
          }
        }
    }

    for (int i = 0; i < HEAP_SIZE; i++)
    {
      if (memarea[i] != '!'){
        return FAILURE;
      }
    }

    for(int i = 2*HEAP_SIZE; i < 3*HEAP_SIZE; i++)
    {
      if (memarea[i] != '!')
      {
        return FAILURE;
      }
    }

    return SUCCESS;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: resize
 * INTEGRITY OR DATA CORRUPTION?
 * randomly resize or realloc
 * MANIFESTATION OF ERROR:
 *
 */
int test20() {
    int n_tries = 10000;
    int block_size;
    char memarea[HEAP_SIZE*3];
    //fill padding with an arbitrary value
    for (int i = 0; i < 3*HEAP_SIZE; i++)
    {
      memarea[i] = '!';
    }
    char *heap_start = &memarea[HEAP_SIZE]; // heap will start 1024 bytes in
    char *pointers[NPOINTERS];
    int block_options[] = {1,5,8,14,23,32,64,71,88,101,223};
    int num_blocks = 11;
    int block_sizes[NPOINTERS];
    char characters[26];
    for (int i = 0; i < 26;i++)
    {
      characters[i] = i+'a';
    }
    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));
    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++)
    {
        int index = random() % NPOINTERS;
        block_size = block_options[random() % num_blocks];
        if (pointers[index] == NULL)
        {
            pointers[index] = hl_alloc(heap_start, block_size);
            if(pointers[index]!= NULL)
            {
              char* array = (char*) pointers[index];
              uintptr_t q1 = (uintptr_t) array;
              if(q1%8 !=0){return FAILURE;}
              block_sizes[index] = block_size;
              for(int j = 0; j < block_size; j++)
              {
                array[j] = characters[index%26];
              }
            }
        }
        else
        {
            char* array2 = (char*) pointers[index];
            for(int q = 0 ; q < block_sizes[index]; q++)
            {
              if(array2[q]!= characters[index%26]){
                return FAILURE;
              }
            }
            if(index % 6 != 0)
            {
              int old_size = block_sizes[index];
              block_size = 1.5* block_options[random() % num_blocks];
              if(old_size > block_size)
              {
                old_size = block_size;
              }
              pointers[index] = hl_resize(heap_start, pointers[index], block_size);
              block_sizes[index] = block_size;

              if (pointers[index] != NULL)
              {
                char* array = (char*) pointers[index];
                uintptr_t q1 = (uintptr_t) array;
                if(q1%8 !=0){return FAILURE;}
                block_sizes[index] = block_size;
                for (int p = 0; p < old_size; p++)
                {
                  if(array[p]!= characters[index%26]){
                    return FAILURE;
                  }
                }
                for(int t = 0; t < block_size; t++)
                {
                  array[t] = characters[index%26];
                }
              }
            }
            else
            {
              hl_release(heap_start, pointers[index]);
              pointers[index] = NULL;
            }
        }
        for(int j = 0; j < NPOINTERS; j++)
        {
          for(int b = 0; b < block_sizes[j]; b++)
          {
            char* array = (char*) pointers[j];
            if(array != NULL)
            {
              if(array[b] != (characters[j%26]))
              {
                return FAILURE;
              }
            }
          }
        }
    }

    for (int i = 0; i < HEAP_SIZE; i++){
      if (memarea[i] != '!')
      {
        return FAILURE;
      }
    }

    for(int i = 2*HEAP_SIZE; i < 3*HEAP_SIZE; i++){
      if (memarea[i] != '!')
      {
        return FAILURE;
      }
    }

    return SUCCESS;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: alloc, init, resize, release
 * INTEGRITY OR DATA CORRUPTION?
 * A variation of spec test 8
 * MANIFESTATION OF ERROR:
 *
 */
int test21() {
  int multiplier;
  for (int j = 0; j< 500;j++)
  {
      multiplier = (random()%10) + 10;
      char heap[HEAP_SIZE*multiplier];
      int block_size = 0;
      if(j%3 == 0){ block_size = 17;}
      else if(j%3==1){block_size = 73;}
      else {block_size = 257;} //43,45,46,48,54
      hl_init(heap, HEAP_SIZE*multiplier);
      char* block_0 = hl_alloc(heap, block_size);
      char* block_2 = hl_alloc(heap, block_size);
      //filling block_0 with p
      for(int i = 0 ; i < block_size; i++)
      {
        block_0[i] = 'p';
        block_2[i] = 't';
      }
      char* block_1 = hl_alloc(heap, block_size*multiplier);
      char* block = hl_alloc(heap, block_size);
      while(block!=NULL)
      {
        for(int i = 0 ; i< block_size; i++)
        {
          block[i] = 'r';
        }
        //printf("filling another block");
        block = hl_alloc(heap, block_size);
      }

      hl_release(heap,block_1); //freeing enough space for hypothetical issues
      block_0 = hl_resize(heap,block_0, (1.5*block_size)); //resizing block
      if(block_0 == NULL){
        //printf("1");
        return FAILURE;
        }//should be able to resize

      for(int k= 0; k < 1.5*block_size;k++)
      {
          if (k < block_size && block_0[k] != 'p') //no segfaults
          {
            //printf("2");
            return FAILURE;
          }
      }
      block_0 = hl_resize(heap,block_0, (0.5*block_size));
      if(block_0 == NULL){
        //printf("3");
        return FAILURE;
        }
      for(int k= 0; k < (int)(0.5*block_size);k++)
      {
          if (block_0[k] != 'p'){
            //printf("4");
            return FAILURE;
            }
      }
      block_2 = hl_resize(heap,block_2,(block_size));//resizing block to same size
      for(int k= 0; k < block_size;k++)
      {
          if (block_2[k] != 't'){
            //printf("5");
            return FAILURE;
            }
      }
      block_2 = hl_resize(heap,block_2,(0.75*block_size));//resizing block to same size
      for(int k= 0; k < (int)(0.75*block_size); k++)
      {
          if (block_2[k] != 't'){
            //printf("6");
            return FAILURE;
            }
      }
  }
  return SUCCESS;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: alloc, release, resize
 * INTEGRITY OR DATA CORRUPTION?
 * random block size, 90% resize, 10% release
 * MANIFESTATION OF ERROR:
 *
 */
int test22() {
    int n_tries = 10000;
    int block_size;
    char memarea[HEAP_SIZE*3];
    //fill padding with an arbitrary value
    for (int i = 0; i < 3*HEAP_SIZE; i++)
    {
      memarea[i] = '!';
    }
    char *heap_start = &memarea[HEAP_SIZE]; // heap will start 1024 bytes in
    char *pointers[NPOINTERS];
    int block_options[] = {1,5,8,14,23,32,64,71,88,101,223};
    int num_blocks = 11;
    int block_sizes[NPOINTERS];
    char characters[26];
    for (int i = 0; i < 26;i++)
    {
      characters[i] = i+'a';
    }
    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));
    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++)
    {
        int index = random() % NPOINTERS;
        block_size = block_options[random() % num_blocks];
        if (pointers[index] == NULL)
        {
            pointers[index] = hl_alloc(heap_start, block_size);
            if(pointers[index]!= NULL)
            {
              char* array = (char*) pointers[index];
              uintptr_t q1 = (uintptr_t) array;
              if(q1%8 !=0){return FAILURE;}
              block_sizes[index] = block_size;
              for(int j = 0; j < block_size; j++)
              {
                array[j] = characters[index%26];
              }
            }
        }
        else
        {
            char* array2 = (char*) pointers[index];
            for(int q = 0 ; q < block_sizes[index]; q++)
            {
              if(array2[q]!= characters[index%26]){
                return FAILURE;
              }
            }
            if(index % 10 < 9)
            {
              block_size = 2.86* block_options[random() % num_blocks];
              pointers[index] = hl_resize(heap_start, pointers[index], block_size);
              block_sizes[index] = block_size;
              if (pointers[index] != NULL)
              {
                char* array = (char*) pointers[index];
                uintptr_t q1 = (uintptr_t) array;
                if(q1%8 !=0){return FAILURE;}
                block_sizes[index] = block_size;
                for(int t = 0; t < block_size; t++)
                {
                  array[t] = characters[index%26];
                }
              }
            }
            else
            {
              hl_release(heap_start, pointers[index]);
              pointers[index] = NULL;
            }
        }
        for(int j = 0; j < NPOINTERS; j++)
        {
          for(int b = 0; b < block_sizes[j]; b++)
          {
            char* array = (char*) pointers[j];
            if(array != NULL)
            {
              if(array[b] != (characters[j%26]))
              {
                return FAILURE;
              }
            }
          }
        }
    }

    for (int i = 0; i < HEAP_SIZE; i++){
      if (memarea[i] != '!')
      {
        return FAILURE;
      }
    }

    for(int i = 2*HEAP_SIZE; i < 3*HEAP_SIZE; i++){
      if (memarea[i] != '!')
      {
        return FAILURE;
      }
    }

    return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test23() {
  int n_tries = 100000;
  int block_size;
  char memarea[HEAP_SIZE*3];
  //fill padding with an arbitrary value
  for (int i = 0; i < 3*HEAP_SIZE; i++)
  {
    memarea[i] = '!';
  }
  char *heap_start = &memarea[HEAP_SIZE]; // heap will start 1024 bytes in
  char *pointers[NPOINTERS];
  int block_options[] = {1,5,8,14,23,32,64,71,88,101,223,240};
  int num_blocks = 12;
  int block_sizes[NPOINTERS];
  memset(block_sizes, 0, NPOINTERS*sizeof(int));
  char characters[26];
  for (int i = 0; i < 26;i++)
  {
    characters[i] = i+'a';
  }
  // zero out the pointer array
  memset(pointers, 0, NPOINTERS*sizeof(char *));
  hl_init(heap_start, HEAP_SIZE);
  srandom(0);
  for (int i = 0; i < n_tries; i++)
  {
      int index = random() % NPOINTERS;
      block_size = block_options[random() % num_blocks];
      if (pointers[index] == 0)
      {
          pointers[index] = hl_alloc(heap_start, block_size);
          if(pointers[index]!= NULL)
          {
            char* array = (char*) pointers[index];
            uintptr_t q1 = (uintptr_t) array;
            if(q1%8 !=0){return FAILURE;}
            block_sizes[index] = block_size;
            for(int j = 0; j < block_size; j++)
            {
              int p = random() %3;
              if (p==1)
              {
                array[j] = characters[index%26];
              }
            }
          }
      }
  }
  for(int k=0; k < NPOINTERS;k++)
  {
    if (pointers[k] != NULL)
    {
      hl_release(heap_start,pointers[k]);
      void* w_all = hl_alloc(heap_start,block_sizes[k]);
      if(w_all == NULL)
      {
        return FAILURE;
      }
    }
  }

  for (int i = 0; i < HEAP_SIZE; i++){
    if (memarea[i] != '!')
    {
      return FAILURE;
    }
  }

  for(int i = 2*HEAP_SIZE; i < 3*HEAP_SIZE; i++){
    if (memarea[i] != '!')
    {
      return FAILURE;
    }
  }

  return SUCCESS;
}
