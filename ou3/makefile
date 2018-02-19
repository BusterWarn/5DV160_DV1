CC = gcc -std=c99
#CFLAGS = -O2 -Wall -Wextra
CFLAGS = -g -fno-omit-frame-pointer -Wall -Wextra

.PHONY: all
all: table-example test-slist test-array test-hash

table-example: table-example.o table-slist.o table.h
	$(CC) $(CFLAGS) -o $@ table-example.o table-slist.o

test-slist: test.o table-slist.o table.h
	$(CC) $(CFLAGS) -o $@ test.o table-slist.o

test-array: test.o table-array.o table.h
	$(CC) $(CFLAGS) -o $@ test.o table-array.o

test-hash: test.o table-hash.o table.h
	$(CC) $(CFLAGS) -o $@ test.o table-hash.o

table-example.o: table-example.c table.h
	$(CC) $(CFLAGS) -c table-example.c

test.o: test.c table.h
	$(CC) $(CFLAGS) -c test.c

table-slist.o: table-slist.c table.h
	$(CC) $(CFLAGS) -c table-slist.c

table-array.o: table-array.c table.h
	$(CC) $(CFLAGS) -c table-array.c

table-hash.o: table-hash.c table.h
	$(CC) $(CFLAGS) -c table-hash.c

.PHONY: clean
clean:
	rm -fv *.o table-example test-slist test-array test-hash
