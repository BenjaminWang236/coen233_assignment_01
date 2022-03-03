# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g		adds debugging information to the executable file
#  -Wall	turns on most, but not all, compiler warnings
#  -fshort-enums	so enum type has the smallest size possible to hold the largest enum value
CFLAGS  = -g -Wall -fshort-enums

# the build target executable:
TARGET = myclient myserver generateTestInputs

HEADER = customProtocol.c

all: $(TARGET)

$(TARGET): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(HEADER)

cs: client server

client: myclient.c
	$(CC) $(CFLAGS) -o myclient myclient.c $(HEADER)

server: myserver.c
	$(CC) $(CFLAGS) -o myserver myserver.c $(HEADER)

generate: generateTestInputs.c
	$(CC) $(CFLAGS) -o generateTestInputs generateTestInputs.C $(HEADER)

clean:
	$(RM) $(TARGET)

_clean:
	$(RM) myclient myserver 