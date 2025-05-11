# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -Iinc -Iinclude -Wall -O2

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Source and object files
SRCS = $(wildcard $(SRCDIR)/*.cpp) main.cpp
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

# Executable name
TARGET = $(BINDIR)/PokerGame

# Default target
all: $(TARGET)

# Create bin and obj directories if they don't exist
$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile .cpp to .o
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Optional: run the game
run: all
	./$(TARGET)
