#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "heaplib.h"

/* -------------------- DEFINITIONS ----------------- */

/* to keep things simple, support only 5 blocks of memory at a time*/
#define N_SUPPORTED_BLOCKS 5
#define BLOCK_NOT_FOUND -1

/* Useful shorthand: casts a pointer to a (char *) before adding */
#define ADD_BYTES(base_addr, num_bytes) (((char *)(base_addr)) + (num_bytes))

typedef struct _block_info_t {
    unsigned int block_size;
    void *block;
} block_info_t;

typedef struct _heap_header_t {
    unsigned int heap_size;
    block_info_t blocks[N_SUPPORTED_BLOCKS];
    bool in_use_f[N_SUPPORTED_BLOCKS]; // true or false?
} heap_header_t ;


/* -------------------- PRINT DEBUG FNS ----------------- */

/* This is an example of a debug print function.  The entire body is
 * wrapped in an #ifdef so that the default compile does not include
 * them.  Print statements create huge output files (for which we have
 * ZERO TOLERANCE) so please wrap all printf statements like this.
 */
void print_debug_heap_header(heap_header_t *header) {
#ifdef PRINT_DEBUG
    printf("heap starts at addr %p\n"   // C printing trick.
           "heap_size = %d\n",            // Notice: no commas between lines
           header, header->heap_size);
    for (int i = 0; i < N_SUPPORTED_BLOCKS; i++) {
        printf("[%d] = [%d,%p,%d]\n", i,
               header->blocks[i].block_size,
               header->blocks[i].block,
               header->in_use_f[i] ?  1 : 0);
    }
#endif
}

void print_debug_block_block(heap_header_t *header, int which_block) {
#ifdef PRINT_DEBUG
    int i, num_chars = header->blocks[which_block].block_size;
    char *block = header->blocks[which_block].block;
    printf("block:\n");
    for (i = 0; i < num_chars; i++) {
        printf("\t%c\n", block[i]);
    }
#endif
}

void print_debug_alloc(void *block_addr) {
#ifdef PRINT_DEBUG
    printf("will give user block beginning @: %p\n", block_addr);
#endif
}

void print_debug_entering_init() {
#ifdef PRINT_DEBUG
    printf("Entering hl_init()\n");
#endif
}

void print_debug_entering_alloc(int size) {
#ifdef PRINT_DEBUG
    printf("Entering hl_alloc(), looking for block of size %d\n", size);
#endif
}

void print_debug_entering_release() {
#ifdef PRINT_DEBUG
    printf("Entering hl_release()\n");
#endif
}

void print_debug_entering_resize() {
#ifdef PRINT_DEBUG
    printf("Entering hl_resize()\n");
#endif
}

/* ------------------- HELPER FUNCTIONS ----------------- */

/* Given the heap header & a block address, finds the block in
 * question. Returns BLOCK_NOT_FOUND if not found.
 *
 */
int find_block(heap_header_t *header, void *block) {

    // searching through blocks, looking for one given
    for (int i = 0; i < N_SUPPORTED_BLOCKS; i++) {
        if (header->blocks[i].block == block) { // found it!
            return i;
        }
    }

    return BLOCK_NOT_FOUND;
}

/* -------------------- THE BIG FOUR FNS ----------------- */

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * Initializes the heap size (after asserting size >= MIN_HEAP_SIZE).
 * Initializes block information for N_SUPPORTED_BLOCKS, which start
 * empty ( = size of 0, NULL pointer, and not in use).
 */
void hl_init(void *heap, unsigned int heap_size) {

    int i;

    print_debug_entering_init();

    heap_header_t *header = (heap_header_t *)heap;
    header->heap_size = heap_size;

    for (i = 0; i < N_SUPPORTED_BLOCKS; i++) {
       header->blocks[i].block_size = 0;
       header->blocks[i].block = NULL;
       header->in_use_f[i] = false;
    }

    print_debug_heap_header(header);

}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * Looks through N_SUPPORTED_BLOCKS, looking for one that is free.  If
 * found, initialize the block and return the location of the block
 * to the user. If not found, return FAILURE.
 */
void *hl_alloc(void *heap, unsigned int block_size) {

    heap_header_t *header = (heap_header_t *)heap;
    int i;

    // begin by pointing to the first block
    void *next_free_byte = ADD_BYTES(header,
                                     sizeof(unsigned int) /* heapsize */ +
                                     sizeof(block_info_t) * N_SUPPORTED_BLOCKS /* block info */ +
                                     sizeof(bool) * N_SUPPORTED_BLOCKS /* in use */);

    print_debug_entering_alloc(block_size);

    // searching through blocks, looking for one not in use
    for (i = 0; i < N_SUPPORTED_BLOCKS; i++) {
        if (!header->in_use_f[i]) {      // found one!
            header->in_use_f[i] = true;  // mark used
            header->blocks[i].block_size = block_size;
            header->blocks[i].block = next_free_byte;
            break;
        } else {
            // if this block is taken, the next block will be "block size" later
            next_free_byte = header->blocks[i].block + header->blocks[i].block_size;
        }
    }
    if (i == N_SUPPORTED_BLOCKS)
        return FAILURE;

    print_debug_alloc(next_free_byte);
    print_debug_heap_header(header);

    return next_free_byte;
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * Initializes the heap size (after asserting size >= MIN_HEAP_SIZE).
 * Initializes block information for N_SUPPORTED_BLOCKS, which start
 * empty ( = size of 0, NULL pointer, and not in use).
 */
void hl_release(void *heap, void *block) {
    heap_header_t *header = (heap_header_t *)heap;

    print_debug_entering_release();
    int i = find_block(header, block);
    header->in_use_f[i] = false;
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * Finds the block that needs to be resized. Changes the size of that block.
 */
void *hl_resize(void *heap, void *block, unsigned int new_size) {

    heap_header_t *header = (heap_header_t *)heap;

    print_debug_entering_resize();

    int i = find_block(header, block);
    header->blocks[i].block_size = new_size;

    return block;

}
