#include <stdlib.h>
#include <stdio.h>

#include "table.h"


//Declaring structs
typedef struct pair {

	KEY key;
	VALUE val;
	struct pair *next;
} pair;


struct table
{
	int size;
	int capacity;
	struct pair **buckets;
	key_compare_func key_cmp;
	key_hash_func hash_key;
};


//Self made help functions functions


//Finds position of pair
//param [in]: key - key of the pair
//return: pointer to location of pair, NULL of pair does not exist
pair *pair_find(table *t, KEY key) {

	int i = t -> hash_key(key) % t -> capacity;
	pair *p = t -> buckets[i];

	while (p  -> next != NULL) {

		if (t -> key_cmp(p -> next -> key, key)) {

			return p;
		}
		p = p -> next;
	}
	return p;
}


//Frees pair
//param [in]: p - pointer to pair
void pair_kill(pair *p) {
	free(p -> val);
	free(p -> key);
	free(p);
}


//Allocates a memory for a pair and gives it key and value
//param [in]: key - key of the pair
//param [in]: val - value of the pair
//return: pointer to where pair is allocated
pair *pair_create(KEY key, VALUE val) {

	pair *p = malloc(sizeof(pair));
	p -> key = key;
	p -> val = val;
	p -> next = NULL;
	return p;
}


//Already created functions


//Creates an empty table
//param [in]: capacity - number of buckets the table can fit
//param [in]: cmp - function pointer to compare different keys
//param [in]: hash - function pointer to hash a key
//return: empty table
table *table_empty(int capacity, key_compare_func cmp, key_hash_func hash)
{
    table *t = malloc(sizeof(table));
	t -> buckets = malloc(capacity * sizeof(pair*));
	for (int i = 0; i < capacity; i++) {
		t -> buckets[i] = malloc(sizeof(pair));
		t -> buckets[i] -> next = NULL;
	}
	t -> size = 0;
	t -> capacity = capacity;
	t -> key_cmp = cmp;
	t -> hash_key = hash;

    return t;
}


//Examines if table is empty
//param [in]: t - table to be examined
//return: bool - true if table is empty, else bool - false
bool table_is_empty(table *t)
{
    return t -> size == 0;
}


//Inserts a new pair into table
//param [in]: t - table to insert value into
//param [in]: key - key of new pair
//param [in]: value - value of new pair
void table_insert(table *t, KEY key, VALUE val)
{
	pair *p = pair_find(t, key);
	if (p -> next == NULL) {

		p -> next = pair_create(key, val);
		t -> size++;
	} else {

		free(p -> next -> key);
		free(p -> next -> val);
		p -> next -> key = key;
		p -> next -> val = val;
	}
}


//Looks up value of a pair in table
//param [in]: t - table to be examined
//param [in]: key - key of the pair
//return: if pair found value of pair, else NULL
VALUE table_lookup(table *t, KEY key)
{
	pair *p = pair_find(t, key);

	if (p -> next != NULL) {

		if (t -> key_cmp(p -> next -> key, key)) {

			return p -> next -> val;
		}
	}
	return NULL;
}


//Removes a pair from a table
//param [in]: t - table to remove from
//param [in]: key - key of the pair that is to be removed
void table_remove(table *t, KEY key)
{
    pair *p = pair_find(t, key);

	if (p -> next != NULL) {

		pair *tmp = p -> next -> next;
		pair_kill(p -> next);
		p -> next = tmp;

		t -> size = t -> size - 1;
	}
}


//Frees the allocated memory of an entire table
//param [in]: t - table that is to be free'd
void table_kill(table *t)
{
    for (int i = 0; i < t -> capacity; i++) {

		pair *p = t -> buckets[i];
		while(p -> next != NULL) {

			pair *tmp = p -> next -> next;
			pair_kill(p -> next);
			p -> next = tmp;
		}
		free(p);
	}

	free(t -> buckets);
    free(t);
}