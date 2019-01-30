/*
Instruction for students
========================

Compile this program using `gcc -o example2 example2.c`

1.) Run this program and enter some stuff in the keyboard. Do you see how your string values are stored in the array? What happens to the space used? How is the end of the string denoted? Using the keyboard, what is the range of characters that you can input into the array?

2.) Using the xxd utility, craft a string with ASCII value of 1's and feed it into the buffer by piping the stream to stdin to the scanf call. Will this cause an error?

3.) It seems that xxd lets us input many different kinds of values into the buffer even though we don't have to change the code. What kind of string pattern are corner cases where we cannot produce even using xxd? String with multiple terminating zeroes? Lots of 0xff? New lines? Really long strings? Try each of these out and see for yourself what is possible and what is not possible.

4.) Suppose now that the scanf call is made without the limitation of 10 bytes. How will that change the answer to question 3? Why?

5.) Suppose now that the assumption in 4 still applies, but we allocate str in the heap. How does that change your answers to 3?

*/

#include "stdio.h"

int main() {
    char str[11];
    int counter;

    printf("Enter a string of length 10:\n");
    scanf("%10s", str);

    printf("Offset\tvalue\tASCII\n");
    for (counter = 0; counter < sizeof(str); counter++) {
        printf("%3d\t%3d\t%c\n", counter, str[counter], str[counter]);
    }
    return 0;
}

