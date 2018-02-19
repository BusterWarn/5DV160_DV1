#include <stdlib.h>
#include <stdio.h>


int main(void)
{
    char *leak = malloc(16);
    int array[4];
    printf("array[0] = %d\n", array[0]);
    char *uaf = malloc(16);
    free(uaf);
    *uaf = 1;
    char *df = malloc(16);
    free(df);
    free(df);
    array[4] = 1;
    leak[16] = 1;
    return 0;
}
