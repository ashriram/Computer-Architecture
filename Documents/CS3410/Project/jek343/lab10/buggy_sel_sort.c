#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

// TODO 1: Debug all the code!

/**
 * Returns the index of the smallest element in long array arr with length len.
 */
int smallest_idx(long *arr, int len)
{
    int i;
    int smallest_i = 0;
    long smallest = arr[0];

    for (i = 1; i < len; i++)
    {
        if (arr[i] < smallest)
        {
            smallest = arr[i];
            smallest_i = i;
        }
    }

    return smallest_i;
}

/**
 * Swaps the values stored at the memory addresses pointed to by a and b.
 */
void swap(long *a, long *b)
{
    long tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 * Performs an in-place selection sort on long array arr with length len.
 */
void selection_sort(long *arr, int len)
{
    int i, swap_idx;

    for (i = 0; i < len; i++)
    {
        swap_idx = i + smallest_idx(&arr[i], len - i);

        swap(&arr[i], &arr[swap_idx]);
    }
}

/**
 * Prints the long array arr with length len to stdout.
 */
void print_array(long *arr, int len)
{
    int i;
    while (i <= len)
    {
        printf("%ld ", arr[i]);
        i++;
    }
    puts("");
}

/**
 * returns: the long value in a string of form "<string>:<long>"
 * If string is not of the correct form then undefined behavior is expected.
 * example: long_of_string("str:42"); returns 42.
 */
long long_of_string(char *string)
{
    // TODO 3: Implement me!
    // HINT: use string functions such as: atol, strchr, strcmp, or strsep
    // Brownie points for Error catching!

    return 0;
}

int main(int argc, char *argv[])
{
    // NOTE: Comment out the following code if you want to test with hard-coded
    // string arrays or long arrays.
    long arr[5] = {1,4,2,0,3};
    selection_sort(arr,5);
    if (argc == 1)
    {
        printf("Exiting with status 1. No File.\n");
        exit(1);
    }

    FILE *stream = fopen(argv[1], "r");

    if (stream == NULL)
    {
        printf("Exiting with status 1, file: '%s' does not exist.\n", argv[1]);
        exit(1);
    }

    char buff[200];
    fread(buff, 1, 200, stream);
    fclose(stream);

    // TODO 2: Separate the string into a list of longs.
    // HINT: Use string parsing functions such as: strtok, or strsep, and
    // use your function `long_of_string`

    // TODO 4: Print the array and compare to the printed array after sorting it.
    // Example string array that could be used:
    // char *sample[] = {"str:1", "str:5", "str:4", "str:2", "str:3"};
}
