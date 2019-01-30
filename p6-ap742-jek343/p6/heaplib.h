#ifndef HEAPLIB_H
#define HEAPLIB_H

#define ALIGNMENT 8
#define MIN_HEAP_SIZE 1024

#define SUCCESS 1
#define FAILURE 0

/* Sets up a new heap beginning at 'heap' of size 'heap_size' (in
 * bytes).  This function does not allocate the heap (of size
 * heap_size) pointed to by heap. That should be done already by
 * the user calling this function.
 *
 * PRECONDITIONS:
 * (1) the allocated heap must be 'heap_size' in bytes
 * (2) heap_size must be >= MIN_HEAP_SIZE bytes.
 * If preconditions are violated, non-graceful failure is acceptable.
 */
void hl_init(void *heap, unsigned int heap_size);


/* Allocates a block of memory of size block_size bytes from the heap starting
 * at 'heap'. Returns a pointer to the block on success; returns
 * 0 if the allocator cannot satisfy the request.
 *
 * PRECONDITIONS:
 * (1) hl_init must have been called exactly once for this heap
 * If preconditions are violated, non-graceful failure is acceptable.
 */
void *hl_alloc(void *heap, unsigned int block_size);
void *hl_alloc2(void *heap, unsigned int block_size);


/* Releases the block of previously allocated memory pointed to by
 * block (which currently resides in the heap pointed to by
 * heap). Acts as a NOP if block == 0.
 *
 * PRECONDITIONS:
 * (1) block must have been returned to the user from a prior
 * call to hl_alloc or hl_resize
 * (2) hl_release can only be called ONCE in association with that
 * prior call to hl_alloc or hl_resize
 * If preconditions are violated, non-graceful failure is acceptable.
 */
void hl_release(void *heap, void *block);


/* Changes the size of the block pointed to by block (that
 * currently resides in the heap pointed to by heap), from its
 * current size to size new_size, returning a pointer to the new
 * block, or 0 if the request cannot be satisfied. The contents of
 * the block should be preserved (even if the location of the
 * block changes -- this will happen when it is not possible to
 * increase the size of the current block but there is room
 * elsewhere on the heap to satisfy the request). If block == 0,
 * function should behave like hl_alloc().
 * PRECONDITIONS:
 * (1) block must have been returned to the user from a prior
 * call to hl_alloc or hl_resize
 * If preconditions are violated, non-graceful failure is acceptable.
 */
void *hl_resize(void *heap, void *block, unsigned int new_size);

#endif /*HEAPLIB_H*/
