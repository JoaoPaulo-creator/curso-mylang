TARGET = a.out

CC = g++
CXXFLAGS = -std=c++23 -g -O1 -Wall -Wextra -fsanitize=address

SRC = $(wildcard *.cpp)
OBJS = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

run:
	./a.out file.my
