CC=gcc
C_FLAGS=-std=c99 -Wall

all: test_bitstring

#UTILS
utils.o: utils.c utils.h
	$(CC) $(C_FLAGS) -c utils.c -o utils.o
	
#BITSTRING
libbitstring.so: utils.o bitstring.c bitstring.h
	$(CC) $(C_FLAGS) -o libbitstring.so -shared utils.o bitstring.c

test_bitstring: clean utils.o libbitstring.so test_bitstring.c
	$(CC) $(C_FLAGS) -c test_bitstring.c -o test_bitstring.o
	$(CC) $(C_FLAGS) utils.o ./libbitstring.so test_bitstring.o -o test_bitstring
	./test_bitstring

#CLEAN
clean:
	rm -f test_bitstring *~ *.o *.a *.so
