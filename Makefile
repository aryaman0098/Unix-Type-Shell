all : shell

shell : main.o LLS.o utils.o dirProc.o exec.o
	gcc -o shell $^

main.o : main.c
	gcc -w -c -g main.c

LLS.o : LLS.c
	gcc -w -c -g LLS.c

utils.o : utils.c
	gcc -w -c -g utils.c

dirProc.o : dirProc.c
	gcc -w -c -g dirProc.c

exec.o : exec.c
	gcc -w -c -g exec.c

clean:
	rm main.o LLS.o utils.o dirProc.o exec.o shell

