LAGS = -Wall

tlast: tlast.c utmplib.c
	$(CC) $(CLFAGS) $@.c -o $@


test:
	make tlast
	rm -rf ./testfiles/*
	./tlast pts/0 -f ./samples/wtmp-a > testfiles/tlast.txt
	last pts/0 -f ./samples/wtmp-a > testfiles/last.txt
	diff testfiles/tlast.txt testfiles/last.txt
