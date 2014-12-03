

CC=gcc

CFLAGS=-Wall -g
LFLAGS=-L/usr/lib -lm -lGL -lglut

EXEC=plot
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

$(EXEC) : $(OBJS)
	$(CC) $^ -o $@ $(LFLAGS)

%.o : %.c
	$(CC) -c $^ -o $@ $(CFLAGS)

clean :
	/bin/rm $(EXEC) *.o
