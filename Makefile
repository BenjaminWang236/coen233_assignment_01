# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET = myclient myserver

all: $(TARGET)

$(TARGET): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

client: myclient.c
	$(CC) $(CFLAGS) -o myclient myclient.c

server: myserver.c
	$(CC) $(CFLAGS) -o myserver myserver.c

clean:
	$(RM) $(TARGET)