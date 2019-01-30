/*
Instruction for students
========================

Compile this program using `gcc -fno-stack-protector -o example1 example1.c`

1.) Run this program with length of 1, 2, and 3. Which one of these are safe, and why?

2.) Run this program with length of 4, 5, and 6. Does it crash? If so, why? If not, does this mean everything is okay?

3.) Run this program with a longer length, say 25. Does it crash? What is the problem?

4.) Suppose that instead of declaring an array, we allocated one dynamically on the heap as follows:

char *str = (char *) malloc(3);

Suppose we still shoved a very long string into this array. Will it have problems? Is it the same problem that you experienced in 1-3? Try it on your machine.

*/

#include "stdio.h"
#include "string.h"

int main() {
    char str[3];
    printf("Put something in this unsafe 3 byte buffer:\n");

    scanf("%s", str);

    printf("This is now in buffer: %s\n", str);
    printf("String length: %d\n", (int) strlen(str));
    return 0;
}

