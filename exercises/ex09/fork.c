/* Example code for Exercises in C.

Copyright 2016 Allen B. Downey
License: MIT License https://opensource.org/licenses/MIT

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <wait.h>


// errno is an external global variable that contains
// error information
extern int errno;
int global_var = 0;


// get_seconds returns the number of seconds since the
// beginning of the day, with microsecond precision
double get_seconds() {
    struct timeval tv[1];

    gettimeofday(tv, NULL);
    return tv->tv_sec + tv->tv_usec / 1e6;
}


void child_code(int i, int heap_var)
{
    static int static_var = 0;
    sleep(i);
    int stack_var = 0;
    printf("Child %d stores global_var here %d, heap_var here %d, stack_var here %d, and static_var here %d.\n", i, &global_var, &heap_var, &stack_var, &static_var);
}

// main takes two parameters: argc is the number of command-line
// arguments; argv is an array of strings containing the command
// line arguments
int main(int argc, char *argv[])
{
    int status;
    pid_t pid;
    double start, stop;
    int i, num_children;
    int heap_var = malloc(sizeof(int));

    // the first command-line argument is the name of the executable.
    // if there is a second, it is the number of children to create.
    if (argc == 2) {
        num_children = atoi(argv[1]);
    } else {
        num_children = 1;
    }

    // get the start time
    start = get_seconds();

    for (i=0; i<num_children; i++) {

        // create a child process
        printf("Creating child %d.\n", i);
        pid = fork();

        /* check for an error */
        if (pid == -1) {
            fprintf(stderr, "fork failed: %s\n", strerror(errno));
            perror(argv[0]);
            exit(1);
        }

        /* see if we're the parent or the child */
        if (pid == 0) {
            child_code(i, heap_var);
            exit(i);
        }
    }

    /* parent continues */
    printf("Hello from the parent.\n");

    for (i=0; i<num_children; i++) {
        pid = wait(&status);

        if (pid == -1) {
            fprintf(stderr, "wait failed: %s\n", strerror(errno));
            perror(argv[0]);
            exit(1);
        }

        // check the exit status of the child
        status = WEXITSTATUS(status);
        printf("Child %d exited with error code %d.\n", pid, status);
    }
    // compute the elapsed time
    stop = get_seconds();
    printf("Elapsed time = %f seconds.\n", stop - start);

    exit(0);
}

/*

global_var is a global variable since it's initialized outside all of the
functions' scopes, and therefore it's in the global data segment.

heap_var is located in the heap since malloc() was called (it was 
dynamically allocated).

stack_var is located on the stack since it's just a local variable in
main().

static_var is a static variable so it's located in the static data 
segment.

When the children processes print out the addresses of each of these
variables, they're all the same. The global_var address is the same, the 
heap_var address is the same, etc. This suggests the children threads
share the same global, static, stack, and heap segments.

Global and static variables are stored in the (static) data segment, 
so this also means the children share the same data segment. I'm not 
sure how to check if the code segments are shared but I know they 
should be since the fork is supposed to create an exact copy, with 
the exception of the process ID.

*/