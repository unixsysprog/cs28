CC = cc
CFLAGS = -Wall -std=c99

tar:
	$(CC) $(CFLAGS) main.c tarc.c -o tarc

clean:
	if [ -f foo.tar ]; then rm foo.tar; fi;

run:
	make clean
	make tar
	./tarc foo.tar test-dir