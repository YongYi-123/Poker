.PHONY: all clean run

CXX = g++
CXXFLAGS = -I ./inc -I ./include -std=c++17 -g -Wall -O3

SRCDIR = src
OBJDIR = obj
TARGET = poker

SRC_MAIN = main.cpp
SRC_SRCS = $(wildcard $(SRCDIR)/*.cpp)
SRCS = $(SRC_MAIN) $(SRC_SRCS)

OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SRC_SRCS))) $(OBJDIR)/main.o

all: $(TARGET)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/main.o: main.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR) $(TARGET).exe

run: $(TARGET)
	@echo "Welcome to Poker!"
	@sleep 1
	@echo "Running the game..."
	@sleep 1
	@./$(TARGET)