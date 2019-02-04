/* Example code for Think OS.

Copyright 2014 Allen Downey
License: GNU GPLv3

*/

#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

int *foo() {
    int i;
    int array[SIZE];

    printf("%p\n", array);

    for (i=0; i<SIZE; i++) {
        array[i] = 42;
    }
    return array;
}

void bar() {
    int i;
    int array[SIZE];

    printf("%p\n", array);

    for (i=0; i<SIZE; i++) {
        array[i] = i;
    }
}

int main()
{
    int i;
    int *array = foo();
    bar();

    for (i=0; i<SIZE; i++) {
        printf("%d\n", array[i]);
    }

    return 0;
}


/* 

Stack Allocated Data section answers

1. The code should create and print the address of an array of five integers 
   (all 42), then create and print the address of an array of five integers 
   (values 0, 1, 2...) then print the original array again in main().
2. There was a warning that foo() returns the address of the local variable
   array. My best guess is that it means the local variable array no longer 
   exists after foo() finishes.
3. I got a segmentation fault when I tried to run the program. I think it's
   because the space allocated to the array was taken back when the foo() 
   function ended, because when I comment out the print statement in main(), 
   there isn't a problem -- there's only a problem when you try to access 
   the values of the array. Also, the arrays in foo() and bar() have the
   same addresses, which again suggests they don't keep their stack space
   after they finish.
4. Nothing different happened except foo() didn't print. When I commented
   out the print statement in main() the program ran without any problems.

Basically, I think the LIFO nature of the stack means functions farther up
shouldn't use the addresses of values lower in the stack (last in) since
the values are scrapped when they're taken off the stack.

*/
