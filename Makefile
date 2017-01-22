# Makefile by tong @ 2017-01-20

OBJS = stupidump.o
TARGET = sdump
CC = gcc
CFLAGS = -g -Wall
LDFLAGS = 

$(TARGET): $(OBJS)
	gcc $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS)

all: $(TARGET)

clean:
	@rm -f $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
