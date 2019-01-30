#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include "heaplib.h"
#include "tests.c"

/* Some ASCII color codes that may be useful for debugging! */
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"
#define RESET   "\x1B[0m"

#define EMPTY_TEST "EMPTY TEST"

int main(int argc, char *argv[]) {

    if (argc <= 1) {
        printf("Usage: %s <test #>\n", argv[0]);
        exit(1);
    }

    int (*tests[])(void) = { test00, test01, test02, test03, test04, test05,  /* 0-15 are SPEC TESTS */
                             test06, test07, test08, test09, test10,
                             test11, test12, test13, test14, test15,
                             test16, test17, test18, test19,                  /* 16-23 are STRESS TESTS */
                             test20, test21, test22, test23 };                

    int test_num = atoi(argv[1]);

    printf("Running Test %d: %s\n", test_num, test_descriptions[test_num]);

    if (test_num < 0 || test_num >= NUM_TESTS) {
        printf("Invalid test # not in (1-%d): %d\n", NUM_TESTS, test_num);
        exit(1);
    }

	// WE NEED THE EMPTY TESTS TO FAIL SO THAT WE DO NOT TRY TO RUN THEM
	// ON ALL THE BROKEN MALLOCS!!!!!!!
	assert(tests[test_num]()); // Should succeed

    printf(GREEN "RESULT: Test %d passed!\n" RESET, test_num);

    return 0;
}

