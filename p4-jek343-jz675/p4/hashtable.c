#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "hashtable.h"

struct hashtable {
    // TODO: define hashtable struct to use linkedlist as buckets
    struct linkedlist **array; 
    int buckets; 
};

/**
 * Hash function to hash a key into the range [0, max_range)
 */
int hash(int key, int max_range) {
    // TODO: feel free to write your own hash function (NOT REQUIRED)
    return key % max_range;
}

hashtable_t *ht_init(int num_buckets) {
    // TODO: create a new hashtable
    hashtable_t* table = (hashtable_t*)malloc(sizeof(hashtable_t)); //allocate space equal to table
    table->array = (linkedlist_t**)malloc(sizeof(linkedlist_t*)*num_buckets); //allocate array space
    table->buckets = num_buckets; 
    for (int i = 0; i<num_buckets; i++) { //go through array and initialize linkedlist
    	table->array[i] = ll_init(); 
    } 
    return table; 
}

void ht_add(hashtable_t *table, int key, int value) {
    // TODO: create a new mapping from key -> value.
    // If the key already exists, replace the value.
    int hashed = hash(key, table->buckets);  //hash value
    ll_add(table->array[hashed], key, value);  //add key to hashed bucket
}

int ht_get(hashtable_t *table, int key) {
    // TODO: retrieve the value mapped to the given key.
    // If it does not exist, return 0
    int hashed = hash(key, table->buckets); //same as ht_add
    int value = ll_get(table->array[hashed], key); 
    return value; 
}

int ht_size(hashtable_t *table) {
    // TODO: return the number of mappings in this hashtable
    int totalsize = 0; 
    for (int i = 0; i<table->buckets; i++) {  //go through buckets and add size of ll.
    	totalsize += ll_size(table->array[i]); 
    } 
    return totalsize; 
}
