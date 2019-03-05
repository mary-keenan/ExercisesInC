/*  This program tests the function

	Written for SoftSys Exercise 3 by Mary Keenan. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "util.h"
#include "minunit.h"



static char *test1() {
    int res = endswith("endswith", "swith");
    char *message = "test1 failed: endswith(\"endswith\", \"swith\") should return 1";
    mu_assert(message, res == 1);
    return NULL;
}

static char * run_test() {
    mu_run_test(test1);
    return NULL;
}

int main(int argc, char **argv) {
    char *result = run_test();
    if (result != NULL) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }

    return result != 0;
}