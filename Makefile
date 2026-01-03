CXX ?= g++
# Default flags: include headers and target C++ standard (overrideable)
CXXFLAGS ?= -Iinclude -std=c++23
TARGET = memory_sim
# Use wildcard so adding/removing sources is automatic
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean




