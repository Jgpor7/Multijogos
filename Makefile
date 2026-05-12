CXX = g++
CXXFLAGS = -Wall -std=c++17

# 1. Ajustado para a pasta 'raylib'
INCLUDES = -I./raylib

# 2. Ajustado para a pasta 'raylib'
LDFLAGS = -L./raylib

TARGET = multijogos
SRCS = $(wildcard *.cpp)
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)