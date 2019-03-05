/*  This program tests the function icmpcode_v4(int code) in util.c;
	It's based off of the test_endswith.c program.

	Written for SoftSys Exercise 3 by Mary Keenan. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "util.h"
#include "minunit.h"

int tests_run = 0;

static char *test_icmpcode_v4() {
    char * res = icmpcode_v4(12);
    char *message = "icmpcode_v4 test failed: icmpcode_v4(12) should return \"host unreachable for TOS\"";
    mu_assert(message, !strcmp(res, "host unreachable for TOS"));
    return NULL; 
}

static char * all_tests() {
    mu_run_test(test_icmpcode_v4);
    return NULL;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != NULL) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
