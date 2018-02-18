CC = cc
CFLAGS = -Wall -std=c99

tarc: main.c tarc.c
	$(CC) $(CFLAGS) main.c tarc.c -o tarc

clean:
	rm -f foo.tar main.o tarc.o 

run:
	make clean
	make tarc
	./tarc foo.tar test-dir
