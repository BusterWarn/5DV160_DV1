#ifndef TABLE_H
#define TABLE_H

#include <stdbool.h>
#include <stddef.h>


typedef void *KEY;
typedef void *VALUE;
typedef bool (*key_compare_func)(KEY, KEY);
typedef size_t (*key_hash_func)(KEY);
typedef struct table table;


/**
 * Create a dynamically allocated empty table.
 *
 * Free resources by calling table_kill().
 *
 * \param [in] capacity The capacity of the underlying data structure (if applicable). 
 * \param [in] cmp The key compare function (tests key equality).
 * \param [in] hash The key hash function (may be NULL if not a hash table).
 * \return A dynamically allocated empty table.
 */
table *table_empty(int capacity, key_compare_func cmp, key_hash_func hash);


/**
 * Check if a table is empty.
 *
 * \param [in] t The table.
 * \return True if the table is empty.
 */
bool table_is_empty(table *t);


/**
 * Insert a new key/value pair into the table.
 *
 * Note: The table assumes ownership of the dynamically allocated key
 * and value and will deallocate them using free() in table_remove()
 * and table_kill().
 *
 * \param [in,out] t The table.
 * \param [in] key The key (dynamically allocated).
 * \param [in] val The value (dynamically allocated).
 */
void table_insert(table *t, KEY key, VALUE val);


/**
 * Lookup the value associated with a given key.
 *
 * \param [in] t The table.
 * \param [in] key The key.
 * \return The value if the key exists, otherwise NULL.
 */
VALUE table_lookup(table *t, KEY key);


/**
 * Remove a specified key (if it exists) from the table.
 *
 * Note: Any matching key/value pairs will be deallocated using
 * free().
 *
 * \param [in,out] t The table.
 * \param [in] key The key to remove.
 */
void table_remove(table *t, KEY key);


/**
 * Free resources allocated previously by table_empty().
 *
 * The table must not be used again after return from this function.
 *
 * Note: All key/value pairs will be deallocated using free().
 *
 * \param [in] t The table to deallocate.
 */
void table_kill(table *t);

#endif
