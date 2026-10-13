CC = gcc
CFLAGS = -Wall -Wextra

all: parent child

parent: src/parent.c
	$(CC) $(CFLAGS) src/parent.c -o parent

child: src/child.c
	$(CC) $(CFLAGS) src/child.c -o child

clean:
	rm -f parent child