util.o: util.h util.c
	gcc -c util.c

trout: trout.h trout.c util.o main.c
	gcc -g -o trout trout.c util.o main.c