# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude -I./vcpkg/installed/x64-windows/include

# Linker flags
LDFLAGS = -static -static-libgcc -static-libstdc++ -L./vcpkg/installed/x64-windows/lib

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
DATA_DIR = data

# Output executable name
TARGET = $(BIN_DIR)/airline_system

# Find all .cpp files in the src directory and its subdirectories
SRCS := $(wildcard $(SRC_DIR)/**/*.cpp)

# Add main.cpp to the list of source files
SRCS += main.cpp

# Convert .cpp files to .o files in the build directory
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Rule to build the final executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $@

# Rule to compile .cpp files into .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run