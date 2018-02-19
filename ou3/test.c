#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>

#include "table.h"


// Dynamically allocate an int.
static int *make_int(int i)
{
    int *p = malloc(sizeof(int));
    *p = i;
    return p;
}


// Dynamically allocate a string.
static char *make_string(char *str)
{
    char *dup = malloc(strlen(str) + 1);
    strcpy(dup, str);
    return dup;
}


// Key compare function for integers.
static bool int_compare(KEY key1, KEY key2)
{
    return *(int*) key1 == *(int*) key2;
}


// Key compare function for strings.
static bool string_compare(KEY key1, KEY key2)
{
    return strcmp((char*) key1, (char*) key2) == 0;
}


// Key hash function for integers.
static size_t int_hash(KEY key)
{
    return *(int*) key;
}


// Key hash function for strings.
static size_t string_hash(KEY key)
{
    size_t hash = 0;
    char *str = (char*) key;
    int len = strlen(str);
    for (int i = 0; i < len; i++)
        hash += (unsigned char) str[i];
    return hash;
}


// Get current time in seconds.
static double now()
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec + 1e-6 * tv.tv_usec;
}


// Shuffles an integer array (Fisher-Yates algorithm).
static void shuffle(int seq[], int n)
{
    for (int i = n - 1; i >= 1; i--)
    {
        int j = rand() % (i + 1);
        int t = seq[i];
        seq[i] = seq[j];
        seq[j] = t;
    }
}


// Generate n unique random numbers.
static void random_sample(int seq[], int n)
{
    for (int i = 0; i < n; i++)
        seq[i] = i;
    shuffle(seq, n);
}


// Measure the time it takes to fill a table with values.
static void measure_insert_speed(table *t, int keys[], int values[], int n)
{
    double start;
    double end;

    // Insert all items
    printf("Measuring the time to insert %d key/value pairs...\n", n);
    start = now();
    for (int i = 0; i < n; i++)
        table_insert(t, make_int(keys[i]), make_int(values[i]));
    end = now();
    printf("  duration: %.6lf seconds\n", end - start);
    printf("\n");
}


// Measure the time it takes to lookup existing keys.
static void measure_existing_lookup_speed(table *t, int keys[], int n)
{
    double start;
    double end;

    printf("Measuring the time to lookup %d existing keys...\n", n);
    start = now();
    for(int i = 0; i < n; i++)
        table_lookup(t, &keys[rand() % n]);
    end = now();
    printf("  duration: %.6lf seconds\n", end - start);
    printf("\n");
}


// Measure the time it takes to repeatedly lookup a subset of the existing keys.
static void measure_existing_subset_lookup_speed(table *t, int keys[], int n)
{
    double start;
    double end;

    int startindex = n / 3;
    int stopindex = 2 * n / 3;
    int size = stopindex - startindex + 1;

    printf("Measuring the time to lookup %d existing keys from a subset of size %d...\n", n, size);
    start = now();
    for (int i = 0; i < n; i++)
        table_lookup(t, &keys[rand() % size + startindex]);
    end = now();
    printf("  duration: %.6lf seconds\n", end - start);
    printf("\n");
}


// Measure the time it takes to lookup non-existing keys.
static void measure_nonexisting_lookup_speed(table *t, int keys[], int n)
{
    double start;
    double end;

    printf("Measuring the time to lookup %d non-existing keys...\n", n);
    start = now();
    int nonexisting_key = -1;
    for (int i = 0; i < n; i++)
        table_lookup(t, &nonexisting_key);
    end = now();
    printf("  duration: %.6lf seconds\n", end - start);
    printf("\n");
}


// Measure the time it takes to remove all keys.
static void measure_remove_speed(table *t, int keys[], int n)
{
    double start;
    double end;

    printf("Measuring the time to remove %d keys...\n", n);
    start = now();
    shuffle(keys, n);
    for (int i = 0; i < n; i++)
        table_remove(t, &keys[i]);
    end = now();
    printf("  duration: %.6lf seconds\n", end - start);
    printf("\n");
}


static void verify(bool condition, char *msg)
{
    if (condition)
    {
        printf("[OK] %s\n", msg);
    } else {
        printf("[FAILED] %s\n", msg);
        exit(EXIT_FAILURE);
    }
}


// Test an empty queue.
static void test_is_empty(void)
{
    table *t = table_empty(4, int_compare, int_hash);
    verify(table_is_empty(t) == true, "Empty table is empty");
    table_kill(t);
}


// Test a single insert.
static void test_insert(void)
{
    table *t = table_empty(4, string_compare, string_hash);
    table_insert(t, make_string("key"), make_string("value"));
    verify(table_is_empty(t) == false, "Table is not empty after insert");
    table_kill(t);
}


// Test lookup of existing key.
static void test_lookup_existing_key(void)
{
    char *key = "hello";
    char *value = "world";
    table *t = table_empty(4, string_compare, string_hash);
    table_insert(t, make_string(key), make_string(value));
    char *lookup_value = (char*) table_lookup(t, key);
    verify(lookup_value != NULL && strcmp(lookup_value, value) == 0, "Lookup existing key");
    table_kill(t);
}


// Test lookup of non-existing key.
static void test_lookup_nonexisting_key(void)
{
    char *key = "hello";
    char *nonexisting_key = "foo";
    char *value = "world";
    table *t = table_empty(4, string_compare, string_hash);
    table_insert(t, make_string(key), make_string(value));
    char *lookup_value = (char*) table_lookup(t, nonexisting_key);
    verify(lookup_value == NULL, "Lookup non-existing key");
    table_kill(t);
}


// Test multiple lookups.
static void test_multiple_lookup(void)
{
    char *keys[] = { "key1", "key2", "key3" };
    char *values[] = { "val1", "val2", "val3" };
    table *t = table_empty(4, string_compare, string_hash);
    for (int i = 0; i < 3; i++)
        table_insert(t, make_string(keys[i]), make_string(values[i]));
    bool ok = true;
    for (int i = 0; i < 3; i++)
    {
        char *lookup_value = table_lookup(t, keys[i]);
        ok = ok && lookup_value != NULL && strcmp(lookup_value, values[i]) == 0;
    }
    verify(ok, "Lookup of multiple existing keys");
    table_kill(t);
}


// Test repeated lookups of the same key.
static void test_repeated_lookup(void)
{
    char *keys[] = { "key1", "key2", "key3" };
    char *values[] = { "val1", "val2", "val3" };
    table *t = table_empty(4, string_compare, string_hash);
    for (int i = 0; i < 3; i++)
        table_insert(t, make_string(keys[i]), make_string(values[i]));
    bool ok = true;
    for (int i = 0; i < 3; i++)
    {
        char *lookup_value = table_lookup(t, keys[1]);
        ok = ok && lookup_value != NULL && strcmp(lookup_value, values[1]) == 0;
    }
    verify(ok, "Repeated lookup of the same key");
    table_kill(t);
}


// Test empty after insert + remove.
static void test_empty_after_insert_remove(void)
{
    table *t = table_empty(4, string_compare, string_hash);
    table_insert(t, make_string("key"), make_string("value"));
    table_remove(t, "key");
    verify(table_is_empty(t) == true, "Table is empty after insert and remove");
    table_kill(t);
}


// Test empty after multiple insert and remove.
static void test_empty_multiple_insert_remove(void)
{
    char *keys[] = { "key1", "key2", "key3" };
    char *values[] = { "val1", "val2", "val3" };
    table *t = table_empty(4, string_compare, string_hash);
    for (int i = 0; i < 3; i++)
        table_insert(t, make_string(keys[i]), make_string(values[i]));
    for (int i = 0; i < 3; i++) {
        table_remove(t, keys[i]);
      }
    verify(table_is_empty(t), "Empty after multiple insert and remove");
    table_kill(t);
}


// Test duplicate inserts.
static void test_duplicate_insert(void)
{
    char *key = "key";
    char *values[] = { "val1", "val2", "val3" };
    table *t = table_empty(4, string_compare, string_hash);
    for (int i = 0; i < 3; i++)
        table_insert(t, make_string(key), make_string(values[i]));
    table_remove(t, key);
    verify(table_is_empty(t), "Empty after duplicate inserts and one remove");
    table_kill(t);
}


// Run all correctness tests.
static void correctness_test(void)
{
    test_is_empty();
    test_insert();
    test_lookup_existing_key();
    test_lookup_nonexisting_key();
    test_multiple_lookup();
    test_repeated_lookup();
    test_empty_after_insert_remove();
    test_empty_multiple_insert_remove();
    test_duplicate_insert();
}


// Runs all speed tests.
static void speed_test(int n)
{
    table *t = table_empty(n, int_compare, int_hash);
    int *keys = malloc(n * sizeof(int));
    int *values = malloc(n * sizeof(int));
    random_sample(keys, n);
    random_sample(values, n);
    measure_insert_speed(t, keys, values, n);
    measure_existing_lookup_speed(t, keys, n);
    measure_existing_subset_lookup_speed(t, keys, n);
    measure_nonexisting_lookup_speed(t, keys, n);
    measure_remove_speed(t, keys, n);
    free(keys);
    free(values);
    table_kill(t);
}


int main(int argc, char *argv[])
{
    int n = 100;
    if (argc >= 2)
    {
        n = atoi(argv[1]);
        if (n <= 0)
            n = 1;
        if (n > 10000000)
            n = 10000000;
    }
    printf("Performing correctness tests...\n");
    correctness_test();
    printf("Passed all correctness tests!\n");
    printf("\n");
    speed_test(n);
    printf("Speed test completed!\n");
    return 0;
}
