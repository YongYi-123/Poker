.PHONY: all clean

# Compiler settings
CXX = g++
CXXFLAGS = -I ./headers -I ./include -std=c++17
WARNINGS = -g -Wall -O2

# Directories
SRCDIR = src
OBJDIR = obj
INCDIR = headers

# Sources and Objects
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Target executable
TARGET = poker

all: $(TARGET)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(TARGET): $(SRCDIR)/main.cpp $(OBJS) | $(OBJDIR)
	$(CXX) $(WARNINGS) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

# Compile rule
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) Makefile
	$(CXX) $(WARNINGS) $(CXXFLAGS) -c $< -o $@
