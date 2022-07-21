
CC		:= gcc
CFLAGS	:= -fno-stack-protector -no-pie

all: src/crashy.c
	$(CC) $(CFLAGS) -o crashy.bin src/crashy.c

clean:
	rm crashy.bin