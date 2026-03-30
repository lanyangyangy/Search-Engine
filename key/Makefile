CXX = g++
CXXFLAGS = -I./Include
SRC_DIR = ./src
SRCS = $(wildcard $(SRC_DIR)/*.cc)
OBJS = $(SRCS:.cc=.o)
TARGET = my_program

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)