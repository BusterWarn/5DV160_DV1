#include <stdio.h>

// Return an integer provided by the user through the terminal.
static int get_integer(void)
{
    int val;
    scanf("%d", &val);
    return val;
}

int main(void)
{
    // Get a positive integer num from the user.
    int num;
    do
    {
        printf("Enter a positive integer: ");
        num = get_integer();
    } while (num < 0);

    // Compute the factorial of num.
    int factorial;
    for (int i = 1; i <= num; i++)
        factorial *= i;

    // Print the result.
    printf("%d! = %d\n", num, factorial);
    
    return 0;
}
