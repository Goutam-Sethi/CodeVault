CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = codevault
SRC = src/main.c src/init.c src/status.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -Iinclude -o $(TARGET)

clean:
	rm -f $(TARGET)