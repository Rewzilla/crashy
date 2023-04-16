
CC		:= gcc
CFLAGS	:= -fno-stack-protector -no-pie

all: src/crashy.c src/parse.c src/parse.h
	$(CC) $(CFLAGS) -o crashy.bin src/crashy.c src/parse.c

clean:
	rm crashy.bin