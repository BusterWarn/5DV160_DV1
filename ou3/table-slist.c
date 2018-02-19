#include <stdlib.h>

#include "table.h"


struct node
{
    struct node *next;
    KEY key;
    VALUE val;
};


static struct node *make_node(KEY key, VALUE val)
{
    struct node *n;
    n = malloc(sizeof *n);
    n->next = NULL;
    n->key = key;
    n->val = val;
    return n;
}


static void free_node(struct node *n)
{
    free(n->key);
    free(n->val);
    free(n);
}


struct table
{
    key_compare_func key_cmp;
    struct node *head;
    int size;
};


table *table_empty(int capacity, key_compare_func cmp, key_hash_func hash)
{
    table *t;
    t = malloc(sizeof *t);
    t->key_cmp = cmp;
    t->head = NULL;
    t->size = 0;
    return t;
}


bool table_is_empty(table *t)
{
    return t->size == 0;
}


void table_insert(table *t, KEY key, VALUE val)
{
    // First remove any duplicates.
    table_remove(t, key);

    // Then insert the new key/value pair.
    struct node *n = make_node(key, val);
    n->next = t->head;
    t->head = n;
    t->size += 1;
}


VALUE table_lookup(table *t, KEY key)
{
    // Search for the first occurrence of key.
    struct node *n = t->head;
    while (n)
    {
        // Test for key equality.
        if (t->key_cmp(key, n->key))
        {
            // Found it.
            return n->val;
        }
        n = n->next;
    }

    // Found nothing.
    return NULL;
}


void table_remove(table *t, KEY key)
{
    // Remove the first (and only) occurrence of key.
    struct node **np = &t->head;
    while (*np)
    {
        // Node at position.
        struct node *n = *np;
        
        // Test for key equality.
        if (t->key_cmp(key, n->key))
        {
            // Found it.
            *np = n->next;
            free_node(n);
            t->size -= 1;
            return;
        }
        np = &n->next;
    }
}


void table_kill(table *t)
{
    struct node *n = t->head;
    while (n)
    {
        struct node *next = n->next;
        free_node(n);
        n = next;
    }
    free(t);
}
