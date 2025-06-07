.PHONY: all clean

# Compiler settings
CXX = g++
CXXFLAGS = -I ./inc -I ./include -std=c++17
WARNINGS = -g -Wall -O3

# Source files and object files
SRCDIR = src
OBJDIR = obj
INCDIR = inc
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Name of the executable
TARGET = poker.exe

# Default target
all: $(TARGET)

# Make obj folder if it doesn't exist
$(OBJDIR):
	@if not exist "$(OBJDIR)" mkdir "$(OBJDIR)"

# Link everything into final executable
$(TARGET): main.cpp $(OBJS) | $(OBJDIR)
	$(CXX) $(WARNINGS) $(CXXFLAGS) $^ -o $@

# Compile each .cpp to .o (object)
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CXX) $(WARNINGS) $(CXXFLAGS) -c $< -o $@

# Clean all built files
clean:
	@if exist "$(OBJDIR)" rmdir /s /q "$(OBJDIR)"
	@if exist "$(TARGET)" del /f /q "$(TARGET)"
# Build and run the executable
run: $(TARGET)
	@echo Welcome to Poker!
	@timeout /t 1 > nul
	@echo Running the game...
	@timeout /t 1 > nul
	@$(TARGET)