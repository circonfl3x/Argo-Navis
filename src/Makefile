bin=argo
CC=gcc
files=$(wildcard *.c)
flags= -lm
devflags = -fsanitize=address -g
testnoobj: clean
	$(CC) $(files) -o $(bin) $(flags) $(devflags)  ; ./$(bin)
testnoobj_nofsan: clean
	$(CC) $(files) -o $(bin) -g ; ./$(bin)

clean:
	rm -rf $(bin) out.c

