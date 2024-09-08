CC = gcc
CFLAGS = -Wall
TARGET = test
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)
