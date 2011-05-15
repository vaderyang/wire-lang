CC=gcc
C_OPTS=-std=c99
WARN_OPTS=-Wall
DBG_OPTS=-ggdb
OPTMZ_OPTS=
DIR_OPTS=-I. -I../bitstring/ -L. -L../bitstring/
COMPILE=$(CC) $(DIR_OPTS) $(LNK_OPTS) $(C_OPTS) $(WARN_OPTS) $(DBG_OPTS) $(OPTMZ_OPTS)
AR=ar rcs

all: libserdes.a libserdes.so test

libserdes.a: utils.c serdes.c
	$(COMPILE) -c utils.c -o utils.o
	$(COMPILE) -c serdes.c -o serdes.o
	$(AR) libserdes.a serdes.o utils.o

libserdes.so: utils.c serdes.c
	$(COMPILE) utils.c serdes.c -lbitstring -shared -o libserdes.so

test:
	$(COMPILE) ../bitstring/libbitstring.so utils.c serdes.c test_serdes.c -o test_serdes
	./test_serdes

clean:
	rm -f test_serdes *~ *.o *.a *.so
