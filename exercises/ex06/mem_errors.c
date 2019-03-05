/* Example code for Exercises in C.

Copyright 2014 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void free_anything(int *p) {
    free(p);
}

int read_element(int *array, int index) {
    if (index >= 0) {
        int x = array[index];
        return x;
    }
    return 0;
}

int main()
{
    int *never_allocated = malloc(sizeof (int));
    int *free_twice = malloc(sizeof (int));
    int *use_after_free = malloc(sizeof (int));
    int *never_free = malloc(sizeof (int));
    int array1[100];
    int *array2 = malloc(100 * sizeof (int));

    // valgrind does not bounds-check static arrays
    read_element(array1, -1);
    read_element(array1, 100);

    // but it does bounds-check dynamic arrays
    read_element(array2, -1);
    // read_element(array2, 100); // array is of length 100

    // and it catches use after free
    *use_after_free = 17;
    free(use_after_free);

    // never_free is definitely lost
    *never_free = 17;

    // the following line would generate a warning
    // free(&never_allocated);

    // but this one doesn't
    free_anything(never_allocated);

    free(free_twice);
    // free(free_twice);

    free (never_free);
    free(array2);

    return 0;
}
