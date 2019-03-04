/*  This program mimics the behavior of the tee command. It takes the
	-a and -p options.

	Written for SoftSys Exercise 3 by Mary Keenan. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char ch;
	char line[80];
    int should_append = 0;
    
    /* set boolean should_append variable if -a option was given */
    while ((ch = getopt(argc, argv, "ap")) != EOF) {
    	switch (ch) {
    		case 'a':
    			should_append = 1;
    			break;
			default:
			fprintf(stderr, "Unknown option: '%s'\n", optarg);
			return 1;
    	}
    }

    /* reset the command line arguments so they don't include the options */
    argc -= optind;
    argv += optind;

    /* check to make sure at least one filename was given */
    if (argc == 0) {
		fprintf(stderr, "No filenames were given");
		return 0;
    }

    /* if option -a was not given, clear everything in each of the given 
    files so we're not appending to existing contents */
    if (!should_append) {
    	for (int i = 0; i < argc; i++) {
    		FILE *file = fopen(argv[i], "w");
    		fclose(file);
    	}
    }

    /* proceed with tee functionality -- print the standard input while 
    also writing it to whatever filenames were given */
    while (scanf("%79[^\n]\n", line) == 1) {
    	printf("%s\n", line);
    	for (int i = 0; i < argc; i++) {
    		FILE *file = fopen(argv[i], "a");
    		fprintf(file, "%s\n", line);
    		fclose(file);
    	}
    }

    return 0;
}


/* 

Reflection: It was easy to figure out how to handle the command line
arguments because the Head First C reading had example code, so that
gave me a good jumping off point. The only bug I really dealt with
was using "w" instead of "a" for writing to the file in the for
loop. Since I open and close the files for each line I write, I was 
erasing the last line I had written each time I opened the file to 
write a new line. I also spent a while trying to figure out how to
implement the -p option because I thought we were supposed to; it
wasn't until I checked to see if the professional implementations
had done it that I realized it was probably unnecessary (since the first
one didn't implement it). I would have saved myself time if I had 
double-checked my understanding of the instructions.

Comparison: The biggest difference I notice between my code and the
first professional implementation is that it has a ton of checks and
potential error messages, like checking to make sure the buffer was
actually allocated space before using it. The only errors I check for 
are unrecognized command-line options and incomplete command-line
arguments (no given filenames). Both professional implementations
implemented the -i option, and the second implementation also included
the -p option. The second implementation uses the signal function to
ignore a potential ctrl-c command to implement the -i option, which
is the first time I've ever seen signal() used before. I had also
never seen warn(), which the first implementation uses to format error
messages. Both solutions are a lot more comprehensive than mine.

*/