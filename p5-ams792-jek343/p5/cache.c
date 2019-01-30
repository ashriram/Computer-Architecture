#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "cache.h"

void **make_2d_matrix(int n_row, int n_col, size_t size)
{
  // malloc an array with n_rows
  void **matrix = malloc(n_row * sizeof(void *));
  // for each element in the array, malloc another array with n_col
  int i;
  for (i = 0; i < n_row; i = i + 1)
  {
    matrix[i] = malloc(n_col * size);
  }

  return matrix;
}

cache_t *make_cache(int capacity, int block_size, int assoc)
{
  cache_t *cache = (cache_t *)malloc(sizeof(cache_t));

  cache->capacity = capacity;     // in Bytes
  cache->block_size = block_size; // in Bytes
  cache->assoc = assoc;           // 1, 2, 3... etc.

  cache->was_dirty_evic = false;

  // Set variables based on equations we found
  cache->n_total_cache_line = capacity / block_size;
  cache->n_set = capacity / (assoc * block_size);
  cache->n_offset_bit = log2(block_size);
  cache->n_index_bit = log2(cache->n_set);
  cache->n_tag_bit = 32 - (cache->n_offset_bit) - (cache->n_index_bit);

  // Create cache tags, dirty bits, and lru_way
  cache->cache_tags = (unsigned long **)make_2d_matrix(cache->n_set, assoc, sizeof(unsigned long));
  cache->dirty_bits = (bool **)make_2d_matrix(cache->n_set, assoc, sizeof(bool));
  cache->lru_way = (int *)malloc((cache->n_set) * sizeof(int));

  // initialize cache tags to 0, dirty bits to false, and LRU bits to 0
  for (int i = 0; i < cache->n_set; i++)
  {
    for (int j = 0; j < assoc; j++)
    {
      cache->cache_tags[i][j] = 0;
      cache->dirty_bits[i][j] = 0;
    }
    cache->lru_way[i] = 0;
  }

  return cache;
}

unsigned long get_cache_tag(cache_t *cache, unsigned long addr)
{
  // Returns cache tag (first n_tag_bit bits)
  return (addr >> (32 - (cache->n_tag_bit)));
}

unsigned long get_cache_index(cache_t *cache, unsigned long addr)
{
  // Returns cache index (middle n_index_bit bits)
  addr = addr >> (cache->n_offset_bit);
  unsigned long mask = pow(2, cache->n_index_bit) - 1;
  return addr & mask;
}

unsigned long get_cache_block_addr(cache_t *cache, unsigned long addr)
{
  // Returns block address (address with offset zeroed out)
  addr = addr >> (cache->n_offset_bit);
  addr = addr << (cache->n_offset_bit);
  return addr;
}

/* this function takes a cache, an address, and a flag as to whether the 
 * access is a load or store.
 * functionality in no particular order:
 * 	(1) look up the address in the cache to determine if this access is
 *      a hit or a miss.
 * 	(2) update the cache_tags if necessary to reflect the new state of the
 *      cache if something was brought into the cache.
 * 	(3) update the dirty_bits if we just made a cache line dirty (stored a value to it)
 *      and set the was_dirty_evic flag accordingly if we evicted a dirty cache line.
 * 	(4) update the lru_way field by making the appropriate call to update_cache_lru().
 * Return true if there was a hit, false if there was a miss.
 * IMPORTANT: Use the "get" helper functions above. They will make your life easier.
 */
bool access_cache(cache_t *cache, unsigned long addr, bool is_load)
{
  // Parsing address
  unsigned long tag = get_cache_tag(cache, addr);
  unsigned long index = get_cache_index(cache, addr);

  // was_dirty_evic is false for all hits
  (cache->was_dirty_evic) = false;

  // n way set associative cache
  for (int way = 0; way < cache->assoc; way++)
  {
    // If we have a hit in the cache, return true
    if ((cache->cache_tags)[index][way] == tag)
    {
      // If it's a store, set the dirty bit to true
      if (!is_load)
      {
        (cache->dirty_bits)[index][way] = true;
      } // Otherwise it remains as it was

      // Must update lru because we touched the data here
      update_cache_lru(cache, index, way);

      // Was a hit, return true
      return true;
    }
  }

  // If we have a miss, update cache_tags,
  // replace LRU way, update LRU way,
  // update dirty_bits, set was_dirty_evic flag accordingly, return false

  // Update cache_tags
  int lru_way = (cache->lru_way[index]);
  (cache->cache_tags)[index][lru_way] = tag;

  // Set was_dirty_evic to whatever the dirty bit was (dirty bit hasn't been changed yet)
  cache->was_dirty_evic = (cache->dirty_bits)[index][lru_way];

  // Update dirty bit: true if store, false if not.
  (cache->dirty_bits)[index][lru_way] = !is_load;

  // Update lru_way
  update_cache_lru(cache, index, lru_way);

  return false; // cache miss should return false
}

/*
 * LRU cannot be maintained with a single counter if there are
 * more than 2 ways. So we'll just use an approximation:
 * 	If there is just one way, the LRU bit is always 0.
 * 	If there are two ways, the LRU bit is always the way you DIDN'T just touch.
 *  If there are more than 2 ways, the LRU bit is always 1 higher (w/wrap-around)
 *  	than the way you just touched.
 * For example, if there are 4 ways, and you touch way 0, then the new LRU should be 1.
 * 		If you touch way 3, the new LRU should be 0.
 * the_set: identifies the set in the cache we're talking about
 * touched_way: identifies the way we just touched.
 */
void update_cache_lru(cache_t *cache, int the_set, int touched_way)
{
  // LRU remains 0
  if (cache->assoc == 1)
    return;

  if (touched_way < (cache->assoc - 1))
  {
    cache->lru_way[the_set] = touched_way + 1;
  }
  else
  {
    cache->lru_way[the_set] = 0;
  }
}
