/* Example code for Think OS.

Copyright 2014 Allen Downey
License: GNU GPLv3

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_CHILDREN 10

/* Print an error message and exit.
*/
void perror_exit(char *s)
{
    perror(s);
    exit(-1);
}

/* Call malloc and exit if it fails.
*/
void *check_malloc(int size)
{
    void *p = malloc(size);
    if (p == NULL) {
        perror_exit("malloc failed");
    }
    return p;
}

/* Structure that contains variables shared between threads.
*/
typedef struct {
    int counter;
} Shared;

/* Allocate the shared structure.
*/
Shared *make_shared()
{
    Shared *shared = check_malloc(sizeof(Shared));
    shared->counter = 0;
    return shared;
}

/* Create a child thread.
*/
pthread_t make_thread(void *(*entry)(void *), Shared *shared)
{
    int ret;
    pthread_t thread;

    ret = pthread_create(&thread, NULL, entry, (void *) shared);
    if (ret != 0) {
        perror_exit("pthread_create failed");
    }
    return thread;
}

/* Wait for a child thread.
*/
void join_thread(pthread_t thread)
{
    int ret = pthread_join(thread, NULL);
    if (ret == -1) {
        perror_exit("pthread_join failed");
    }
}

/* Code run by the child threads.
*/
void child_code(Shared *shared)
{
    printf("counter = %d\n", shared->counter);
    // int i;
    // printf("the address of stack variable i is: %p.\n", &i);
    shared->counter++;
}

/* Entry point for the child threads.
*/
void *entry(void *arg)
{
    Shared *shared = (Shared *) arg;
    child_code(shared);
    pthread_exit(NULL);
}

int main()
{
    int i;
    pthread_t child[NUM_CHILDREN];

    Shared *shared = make_shared();

    for (i=0; i<NUM_CHILDREN; i++) {
        child[i] = make_thread(entry, shared);
    }

    for (i=0; i<NUM_CHILDREN; i++) {
        join_thread(child[i]);
    }

    printf("Final value of counter is %d\n", shared->counter);
    return 0;
}


/*
QUESTION TWO
While the intermediary values of the counter don't show it increasing
(they're all 0, which means they all printed before any of the other
threads had increased the counter), the final value is correct 
(it's 5).

QUESTION THREE
When I increase the number of threads to 10, the final value is still
correct at 10, but some of the children print out a non-zero counter
which indicates that some of the children had already changed the counter
before the rest printed, which is evidence of concurrency. There are 
synchronization erroras since it reads (prints) the counter incorrectly 
(because multiple threads are reading or changing it at the same time).

Since the final value of counter is the number of threads, it's clear
all of the threads share the same heap. If they were just using copies
of the original counter variable, each counter copy would only ever
reach the value of 1.

You can tell that threads share the global segment by printing the
address of a global variable in child_code(), which will be the same
across threads.

You can tell that threads don't share the stack segment by printing
the address of a local variable created in child_code(). The 
addresses are not the same and they're not near each other, which means
they're located in different stack segments. The difference between 
the addresses of the local variable indicate how big each thread's 
stack segment is.
*/