CC = gcc
CFLAGS = -Wall
TARGET = server clienta

all: $(TARGET)

%: %.c
	$(CC) $(CFLAGS) -o $@ $^
server: server.c
	$(CC) $(CFLAGS) -o server server.c
clean:
	rm -f $(TARGET)
