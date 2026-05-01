CXX = g++
CXXFLAGS = -Wall -std=c++17

# 1. Diga ao compilador para procurar headers na sua pasta
INCLUDES = -I./lib

# 2. Diga ao linker para procurar a biblioteca (.a) na sua pasta
LDFLAGS = -L./lib

TARGET = multijogos
SRCS = $(wildcard *.cpp)
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)