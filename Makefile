TARGET = cloud-sim

SRC = $(wildcard *.cpp) $(wildcard schedulers/*.cpp)

CXX = g++-14
CXX_VER = -std=c++20
INCLUDES = -I. -Ischedulers

CXXFLAGS_RELEASE = $(CXX_VER) $(INCLUDES) -O2
CXXFLAGS_DEBUG = $(CXX_VER) $(INCLUDES) -Wall -Wextra -pedantic -O0 -fsanitize=address -fsanitize=undefined

all: release

debug: $(SRC)
	$(CXX) $(CXXFLAGS_DEBUG) -o $(TARGET) $(SRC)

release: $(SRC)
	$(CXX) $(CXXFLAGS_RELEASE) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean