# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g		adds debugging information to the executable file
#  -Wall	turns on most, but not all, compiler warnings
#  -fshort-enums	so enum type has the smallest size possible to hold the largest enum value
CFLAGS  = -g -Wall -fshort-enums

# the build target executable:
HEADER = customProtocol
TARGET = myclient myserver generateTestInputs


all: $(TARGET)

$(TARGET): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(HEADER).c

cs: client server

client: myclient.c
	$(CC) $(CFLAGS) -o myclient myclient.c $(HEADER).c

server: myserver.c
	$(CC) $(CFLAGS) -o myserver myserver.c $(HEADER).c

generator: generateTestInputs.c
	$(CC) $(CFLAGS) -o generateTestInputs generateTestInputs.C $(HEADER).c

clean:
	$(RM) $(TARGET)

_clean:
	$(RM) myclient myserver 