# tool macros
CXX := g++
CXXFLAGS := -g -std=c++20 -Wall -Wextra -Werror -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT
LDFLAGS := -lpipewire-0.3

project_name := better-pwmon
TARGET = $(project_name)

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
