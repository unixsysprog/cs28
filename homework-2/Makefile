tar:
	cc -Wall -std=c99 main.c -o tarc

clean:
	if [ -f foo.tar ]; then rm foo.tar; fi;

run:
	make clean
	make tar
	./tarc foo.tar test-dir