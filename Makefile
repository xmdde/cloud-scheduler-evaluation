TARGET = cloud-sim

SRC = *.cpp

CXX = g++-14
CXX_VER = -std=c++17
CXXFLAGS_RELEASE = $(CXX_VER) -O2
CXXFLAGS_DEBUG = $(CXX_VER) -Wall -Wextra -pedantic -O0 -fsanitize=address -fsanitize=undefined

all: $(TARGET)

debug: $(SRC)
	$(CXX) $(CXXFLAGS_DEBUG) -o $(TARGET) $(SRC)

release: $(SRC)
	$(CXX) $(CXXFLAGS_RELEASE) -o $(TARGET) $(SRC)

$(TARGET): release

clean:
	rm -f $(TARGET)

.PHONY: all clean