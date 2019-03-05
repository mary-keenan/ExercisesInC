util.o: util.h util.c
	gcc -c -Wall util.h util.c

trout: trout.h trout.c util.o main.c
	gcc -g -o trout trout.c util.o main.c 
	# when I add -Wall here, I get "error adding symbols: Bad value" and
	# I can't figure out why since it compiles cleanly otherwise

test_endswith: endswith.h endswith.c test_endswith.c minunit.h
	gcc -g -o test_endswith endswith.h endswith.c test_endswith.c minunit.h

test_util: util.h util.c test_util.c minunit.h
	gcc -g -o test_util util.h util.c test_util.c minunit.h