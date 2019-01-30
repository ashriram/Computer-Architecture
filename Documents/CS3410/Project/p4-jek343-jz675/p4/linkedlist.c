#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include <stdbool.h>

struct linkedlist {
    struct linkedlist_node *first;
    // TODO: define linked list metadata
    int length;
};

typedef struct linkedlist_node {
    // TODO: define the linked list node
    int key;
    int value;
    struct linkedlist_node *next;
} linkedlist_node_t;

linkedlist_t *ll_init() {
    // TODO: create a new linked list
    linkedlist_t* list = (linkedlist_t*)malloc(sizeof(linkedlist_t));
    list -> first = NULL;
    list -> length = 0;
    return list;
}

void ll_add(linkedlist_t *list, int key, int value) {
    // TODO: create a new node and add to the front of the linked list if a
    // node with the key does not already exist.
    // Otherwise, replace the existing value with the new value.
    bool dne = true;
    linkedlist_node_t* curr_node = list -> first;
    while(curr_node != NULL){
        if(curr_node -> key == key){
            curr_node -> value = value;
            dne = false;
	}
        curr_node = curr_node -> next;
    }
    if(dne){
        linkedlist_node_t* next = list -> first;
        list -> first = (linkedlist_node_t*)malloc(sizeof(linkedlist_node_t));
	list -> first -> key = key;
	list -> first -> value = value;
	list -> first -> next = next;
	list -> length = (list -> length) + 1;
    }
}

int ll_get(linkedlist_t *list, int key) {
    // TODO: go through each node in the linked list and return the value of
    // the node with a matching key.
    // If it does not exist, return 0.
    linkedlist_node_t* curr_node = list -> first;
    while(curr_node != NULL){
        if(curr_node -> key == key){
            return curr_node -> value;
	}
	curr_node = curr_node -> next;
    }
    return 0;
}

int ll_size(linkedlist_t *list) {
    // TODO: return the number of nodes in this linked list
    return list -> length;
}
