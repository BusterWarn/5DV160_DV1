#include <stdlib.h>

#include "table.h"


//Declaring structs
typedef struct pair {

	KEY key;
	VALUE val;
	struct pair *next;
} pair;


typedef struct bucket {

	struct pair *next;
} bucket;

struct table
{
	int size;
	int capacity;
	struct bucket *buckets;
	key_compare_func key_cmp;
	key_hash_func hash_key;
};


//Self made functions


//Finds position of pair
//param [in]: key - key of the pair
//return: pointer to location of pair, NULL of pair does not exist
pair *pair_find(table *t, KEY key) {

	printf("pair_find - ");
	int hash = t -> hash_key(key);
	int i = hash % t -> capacity;
	pair *p = t -> buckets[i].next;
	
	if (p != NULL) {
		printf("I love cocaine\n");
		while (p -> next != NULL && !t -> key_cmp(p -> next -> key, key)) {
			p = p -> next;
		}
	}

	printf("function ends\n");
	return p;
}


//Frees next pair, relinks pair to thrid one
//param [in]: p - pointer to pair
void pair_kill(pair *p) {

	if (p -> next != NULL) {

		pair* tmp = p -> next -> next;
		free(p -> next -> val);
		free(p -> next -> key);
		free(p -> next);
		p -> next = tmp;
	}
}


//Inserts a pair into list of pair springing out from bucket
//param [in]:
//param [in]:
//return:
void pair_create(pair* p, KEY key, VALUE val) {

	printf("pair_create - ");
	p = malloc(sizeof(pair));
	p -> key = key;
	p -> val = val;
	p -> next = NULL;
	printf("end of function \n");
}

//Already created functions


table *table_empty(int capacity, key_compare_func cmp, key_hash_func hash)
{
    table *t = malloc(sizeof(table));
	t -> buckets = malloc(capacity * sizeof(bucket*));
	for (int i = 0; i < capacity; i++) {
		t -> buckets[i].next = NULL;
	}
	t -> size = 0;
	t -> capacity = capacity;
	t -> key_cmp = cmp;
	t -> hash_key = hash;

    return t;
}


bool table_is_empty(table *t)
{
    return t -> size == 0;
}


void table_insert(table *t, KEY key, VALUE val)
{
	pair *p = pair_find(t, key);
	if (p == NULL) {
		
		printf("WUBBUALAUIBA DUB DUB\n");
		pair_create(p, key, val);
		printf("operation succesfull: %d\n", p != NULL);
		t -> size++;

	} else {

		free(p -> key);
		free(p -> val);
		p -> key = key;
		p -> val = val;
	}
}


VALUE table_lookup(table *t, KEY key)
{
	pair *p;
	printf("capacity : %d\n", t -> capacity);
    for (int i = 0; i < t -> capacity; i++) {
		printf("bucket nr: %d, is NULL: %d\n", i, t -> buckets[i].next == NULL);
		p = pair_find(t, key);
		
		if (p != NULL) {
			if (t -> key_cmp(p -> next -> key, key)) {
				
				return p -> next -> val;
			}
		}
	}
	
    return NULL;
}


void table_remove(table *t, KEY key)
{
    // TODO
}


void table_kill(table *t)
{
    // TODO
    free(t);
}
