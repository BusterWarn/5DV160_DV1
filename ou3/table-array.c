#include <stdlib.h>

#include "table.h"


//Declaring structs
typedef struct pair {

	KEY key;
	VALUE val;
} pair;

struct table
{
	pair** pairs;
	int capacity;
	int current_size;
	key_compare_func key_cmp;
	key_hash_func hash_key;
};


//Self made functions

//finds key in table
//param [in]: key - the key that is checked with table
//param [in]: t - the table that is compared with key
//return: -1 if key is found, else position of key
int find_key(KEY key, table *t) {

	for (int i = 0; i < t -> current_size; i++) {

		if (t -> key_cmp(t -> pairs[i] -> key, key)) {

			return i;
		}
	}

	return -1;
}


//creates a new pair
//param [in]: key - key of the new pair
//param [in]: value - the value of the pair
//return: pointer to where oair is allocated in heap
pair *pair_create(KEY key, VALUE value) {

	pair *p = malloc(sizeof(pair));
	p -> key = key;
	p -> val = value;

	return p;
}


//Frees a pairs val and key
//param [in]: t - table where adress of pair is located
//param [in]: pos - position of adress in table's array
void pair_free(table *t, int pos) {

	free(t -> pairs[pos] -> val);
	free(t -> pairs[pos] -> key);
	free(t -> pairs[pos]);
}


//Already created functions

table *table_empty(int capacity, key_compare_func cmp, key_hash_func hash)
{
    table *t = malloc(sizeof(table));
    t -> pairs = malloc(capacity * sizeof(pair*));
	t -> capacity = capacity;
	t -> current_size = 0;
	t -> key_cmp = cmp;
	t -> hash_key = NULL;

    return t;
}


bool table_is_empty(table *t)
{
    return t -> current_size == 0;
}


void table_insert(table *t, KEY key, VALUE val)
{
	int pos = find_key(key, t);
	if (pos >= 0) {

		pair_free(t, pos);
		t -> pairs[pos] = pair_create(key, val);

	} else if (t -> current_size < t -> capacity) {

		t -> pairs[t -> current_size] = pair_create(key, val);
		t -> current_size++;
	}
}


VALUE table_lookup(table *t, KEY key)
{
	for (int i = 0; i < t -> current_size; i++) {

			if (t -> key_cmp(t -> pairs[i] -> key, key) == 1) {

				return t -> pairs[i] -> val;
			}
	}
    return NULL;
}


void table_remove(table *t, KEY key)
{
	int pos = find_key(key, t);

	if (pos >= 0) {

		pair_free(t, pos);

		t -> pairs[pos] = t -> pairs[t -> current_size - 1];
		t -> current_size--;;
	}
}


void table_kill(table *t)
{
    for (int i = 0; i < t -> current_size; i++) {

		pair_free(t, i);
	}
	free(t -> pairs);
    free(t);
}
