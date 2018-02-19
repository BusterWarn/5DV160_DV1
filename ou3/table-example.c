#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "table.h"


static bool string_cmp(KEY key1, KEY key2)
{
    return strcmp(key1, key2) == 0;
}


static char *duplicate_string(char *str)
{
    int len = strlen(str);
    char *dup = malloc(len + 1);
    strcpy(dup, str);
    return dup;
}


int main(void)
{
    table *t;
    t = table_empty(10, string_cmp, NULL);
    table_insert(t, duplicate_string("Hello"), duplicate_string("World"));
    table_insert(t, duplicate_string("Foo"), duplicate_string("Bar"));
    VALUE val1 = table_lookup(t, "Hello");
    VALUE val2 = table_lookup(t, "Bar");
    if (val1)
        printf("Hello has value %s\n", (char*) val1);
    if (val2)
        printf("Bar has value %s\n", (char*) val2);
    table_remove(t, "Hello");
    table_kill(t);
    return 0;
}
