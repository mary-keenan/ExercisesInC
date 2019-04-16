/* Example code for Think OS.

Copyright 2014 Allen Downey
License: GNU GPLv3

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mutex.h"

#define NUM_CHILDREN 5

void perror_exit(char *s)
{
    perror(s);
    exit(-1); 
} 

void *check_malloc(int size)
{
    void *p = malloc(size);
    if (p == NULL) {
        perror_exit("malloc failed");
    }
    return p;
}

typedef struct {
    int counter;
    int end;
    int *array;
    Mutex *mutex;
} Shared;

Shared *make_shared(int end)
{
    int i;
    Shared *shared = check_malloc(sizeof(Shared));

    shared->counter = 0;
    shared->end = end;
    shared->mutex = make_mutex();

    shared->array = check_malloc(shared->end * sizeof(int));
    for (i=0; i<shared->end; i++) {
        shared->array[i] = 0;
    }
    return shared;
}

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

void join_thread(pthread_t thread)
{
    int ret = pthread_join(thread, NULL);
    if (ret == -1) {
        perror_exit("pthread_join failed");
    }
}

void child_code(Shared *shared)
{
    // printf("Starting child at counter %d\n", shared->counter);

    while (1) {
        if (shared->counter >= shared->end) {
            return;
        }

        mutex_lock(shared->mutex);
        shared->array[shared->counter]++;
        shared->counter++;

        if (shared->counter % 10000 == 0) {
            // printf("%d\n", shared->counter);
        }
        mutex_unlock(shared->mutex);
    }
}

void *entry(void *arg)
{
    Shared *shared = (Shared *) arg;
    child_code(shared);
    // printf("Child done.\n");
    pthread_exit(NULL);
}

void check_array(Shared *shared)
{
    int i, errors=0;

    // printf("Checking...\n");

    for (i=0; i<shared->end; i++) {
        if (shared->array[i] != 1) errors++;
    }
    // printf("%d errors.\n", errors);
}

int main()
{
    int i;
    pthread_t child[NUM_CHILDREN];
    Shared *shared = make_shared(1000000);

    for (i=0; i<NUM_CHILDREN; i++) {
        child[i] = make_thread(entry, shared);
    }

    for (i=0; i<NUM_CHILDREN; i++) {
        join_thread(child[i]);
    }

    check_array(shared);
    return 0;
}

/*
QUESTION TWO
There are synchronization errors, because approximately 1/10th of the
printed values are not cleanly divisble by 10,000. This means that when
the value was checked in the if statement, the counter was cleanly 
divisble by 10,000, but by the time it got to printing the value on the
next line, the counter value had been increased by one or more other
threads to a value that wasn't cleanly divisble by 10,000. The counter
should have been locked before the if statement and until it got
printed.

(confirmed there are no sync errors now)

QUESTION FOUR
I changed the number of children for both programs to 5 to make any
differences in runtime more obvious.

counter_array (.075, .23, .004 for real, user, and sys respectively) 
ran much faster than my_counter_array_mutex (.151, .223, .287 for real,
user, and sys respectively) in all but one of the categories (user);
they were nearly equal in the user time metric. The overall speed
advantage of counter_array indicates that synchronization adds a lot of
overhead that slows the program down.
*/