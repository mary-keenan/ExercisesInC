/* Example code for Think OS.

Copyright 2014 Allen Downey
License: GNU GPLv3

*/

#include <stdio.h>
#include <stdlib.h>

int var1;
void print_address();

int main ()
{
    int var2 = 5;
    void *p = malloc(128);
    char *s = "Hello, World";

    printf ("Address of main is %p\n", main);
    printf ("Address of var1 is %p\n", &var1);
    printf ("Address of var2 is %p\n", &var2);
    printf ("p points to %p\n", p);
    printf ("s points to %p\n", s);

    // when a second call to malloc is made, the heap grows
    void *new_p = malloc(128);
    char *new_s = "Hey, World";
    printf ("p2 points to %p\n", new_p);
    printf ("s2 points to %p\n", new_s);

    // the stack size was lower for local_variable
    print_address();

    // there is a difference of 30 hex / 48 decimal between them
    void *random_chunk_one = malloc(27);
    void *random_chunk_two = malloc(27);
    printf ("random_chunk_one points to %p\n", random_chunk_one);
    printf ("random_chunk_two points to %p\n", random_chunk_two);

    return 0;
}


/*  Prints the address of a local variable. 
*/
void print_address() {
	int local_variable = 12345678910;
	printf("address of local_variable: %p\n", &local_variable);
}