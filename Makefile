CC = g++
CFLAGS = -Wall -Wextra -std=c++11
SRC = src/main.cpp
TARGET = curve

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)