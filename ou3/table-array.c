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


//Self made help functions functions

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
//return: pointer to where pair is allocated
pair *pair_create(KEY key, VALUE value) {

	pair *p = malloc(sizeof(pair));
	p -> key = key;
	p -> val = value;

	return p;
}


//Frees a pairs val and key
//param [in]: t - table where adress of pair is located
//param [in]: pos - position of adress in table's array
void pair_kill(table *t, int pos) {

	free(t -> pairs[pos] -> val);
	free(t -> pairs[pos] -> key);
	free(t -> pairs[pos]);
}


//Already created functions


//Creates an empty table
//param [in]: capacity - number of pairs the table can fit
//param [in]: cmp - function pointer to compare different keys
//param [in]: hash - function pointer to hash a key
//return: empty table
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


//Examines if table is empty
//param [in]: t - table to be examined
//return: bool - true if table is empty, else bool - false
bool table_is_empty(table *t)
{
    return t -> current_size == 0;
}


//Inserts a new pair into table
//param [in]: t - table to insert value into
//param [in]: key - key of new pair
//param [in]: value - value of new pair
void table_insert(table *t, KEY key, VALUE val)
{
	int pos = find_key(key, t);
	if (pos >= 0) {

		pair_kill(t, pos);
		t -> pairs[pos] = pair_create(key, val);

	} else if (t -> current_size < t -> capacity) {

		t -> pairs[t -> current_size] = pair_create(key, val);
		t -> current_size++;
	}
}


//Looks up value of a pair in table
//param [in]: t - table to be examined
//param [in]: key - key of the pair
//return: if pair found value of pair, else NULL
VALUE table_lookup(table *t, KEY key)
{
	for (int i = 0; i < t -> current_size; i++) {

			if (t -> key_cmp(t -> pairs[i] -> key, key) == 1) {

				return t -> pairs[i] -> val;
			}
	}
    return NULL;
}


//Removes a pair from a table
//param [in]: t - table to remove from
//param [in]: key - key of the pair that is to be removed
void table_remove(table *t, KEY key)
{
	int pos = find_key(key, t);

	if (pos >= 0) {

		pair_kill(t, pos);

		t -> pairs[pos] = t -> pairs[t -> current_size - 1];
		t -> current_size--;;
	}
}


//Frees the allocated memory of an entire table
//param [in]: t - table that is to be free'd
void table_kill(table *t)
{
    for (int i = 0; i < t -> current_size; i++) {

		pair_kill(t, i);
	}
	free(t -> pairs);
    free(t);
}
