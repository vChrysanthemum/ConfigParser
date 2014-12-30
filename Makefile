testobjects = test.o config.o

CC:=$(shell sh -c 'type $(CC) >/dev/null 2>/dev/null && echo $(CC) || echo gcc')
all:test

test:$(testobjects)
	$(CC) -o test $(testobjects)

test.o:test.c config.h
	$(CC) -c -o test.o test.c

config.o:config.c config.h
	$(CC) -c -o config.o config.c

clean:
	rm -f *.o *.a test
