/* Example from Head First C.

Downloaded from https://github.com/twcamper/head-first-c

Modified by Allen Downey.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <wait.h>


void error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <search phrase>\n", argv[0]);
        return 1;
    }
    const char *PYTHON = "/usr/bin/python2";
    const char *SCRIPT = "rssgossip.py";
    char *feeds[] = {
        "http://www.nytimes.com/services/xml/rss/nyt/Africa.xml",
        "http://www.nytimes.com/services/xml/rss/nyt/Americas.xml",
        "http://www.nytimes.com/services/xml/rss/nyt/MiddleEast.xml",
        "http://www.nytimes.com/services/xml/rss/nyt/Europe.xml",
        "http://www.nytimes.com/services/xml/rss/nyt/AsiaPacific.xml"
    };
    int num_feeds = 5;
    char *search_phrase = argv[1];
    char var[255];
    int status;
    pid_t pid;

    for (int i = 0; i < num_feeds; i++) {
        sprintf(var, "RSS_FEED=%s", feeds[i]);
        char *vars[] = {var, NULL};

        /* since execle() replaces the current process, this program 
        terminates the first time it calls execle(), which means it 
        only ever gets articles from the first feed; so, we have to
        fork the process for each feed and have the children call 
        execle() instead of the parent */
        pid = fork();

        /* check to see if the fork was successful */
        if (pid == -1) {
            error("Process could not be forked");
        }

        /* check to see if the program running this code is the child
        process (because the child and parent processes are going to do
        different things) */
        if (!pid) { /* this is equivalent to pid == 0 */
            int res = execle(PYTHON, PYTHON, SCRIPT, search_phrase, NULL, vars);
            if (res == -1) {
                error("Child was unable to run the script");
            }
            exit(i); /* only the parent process should run the code below */
        }
    }

    /* the parent process checks the exit status of each child */
    for (int i = 0; i < num_feeds; i++) {
        pid = wait(&status);

        if (pid == -1) {
            error("Parent was unable to wait for a child process");
            perror(argv[0]);
            exit(1);
        }

        /* check whether the child exited successfully or with an
        error code, and print its final status */
        if (WIFEXITED(status)) {
            status = WEXITSTATUS(status);
            printf("Child %d exited successfully\n", pid);
        } else if (WIFSIGNALED(status)) {
            status = WTERMSIG(status);
            printf("Child %d exited with error %d\n", pid, status);
        }
    }

    return 0;
}
