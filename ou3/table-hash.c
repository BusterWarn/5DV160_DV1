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
	struct pair *pos;
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
	pair *p = t -> buckets[i];

	if (p != NULL) {
		while (p != NULL && !t -> key_cmp(p -> key, key)) {
			p = p -> next;
		}
	}

	printf("function ends, returning adress %x\n", p);
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
pair *pair_create(pair* p, KEY key, VALUE val) {

	printf("pair_create - ");
	p = malloc(sizeof(pair));
	p -> key = key;
	p -> val = val;
	p -> next = NULL;
	printf("end of function \n");
	return p;
}


//Already created functions


table *table_empty(int capacity, key_compare_func cmp, key_hash_func hash)
{
        table *t = malloc(sizeof(table));
	t -> buckets = malloc(capacity * sizeof(pair*));
	for (int i = 0; i < capacity; i++) {
		t -> buckets[i] = NULL;
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
	printf("\n\n table_insert\n");
	t -> pos = pair_find(t, key);
	if (t -> pos == NULL) {

		int hash = t -> hash_key(key) % t -> capacity;
		t -> buckets[hash] = pair_create(t -> pos, key, val);
		t -> size++;

	} else {

		free(t -> pos -> key);
		free(t -> pos -> val);
		t -> pos -> key = key;
		t -> pos -> val = val;
	}
}


VALUE table_lookup(table *t, KEY key)
{
	printf("table_lookup\n");
	pair *p = NULL;

        for (int i = 0; i < t -> capacity; i++) {

		if (t -> buckets[i] != NULL) {
			printf("woooo\n");
			printf("bucket key: %d\n", t -> buckets[i] -> key);

			p = pair_find(t, key);
			if (p != NULL) {
				printf("val: %s\n", p -> val);
				return p -> val;
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
