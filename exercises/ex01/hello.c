#include <stdio.h>

int main() {
	int x = 5;
	int y = x + 1;
    printf("Hello, World. Number: %i\n", y);
    return 0;
}



/* 
Optimization section answers

It seems like the line that corresponds to int x = 5 is: 
	movl	$5, -4(%rbp)	
since it's the only line that has a 5 in it.

When I use the O2 flag, the output gets more complicated.
I can no longer tell which line is the declaration int x = 5
since there aren't any lines with the number 5 in them.
My guess is that the program isn't writing the value of x
into memory since it never gets used.

When I use the O2 flag while printing the x, the line 	
	movl	$5, %edx
is there. I think this is the line where x is declared, and I guess
the program decides it actually wants to remember the value of x
since it has to print it.

When I use the O2 flag while printing the y, the line 
	movl	$6, %edx
is there. In the non-optimized version, there's a line for the declaration
of x and the addition of 1 for y. In the optimized version, it only
keeps track of the value of y (6) since that's the value it's printing.

Basically, optimization tries to only hold onto the pieces of information 
that are relevant to the final step of any operation (like printing), 
and in the most condensed form possible (so simplified if math is involved).

*/