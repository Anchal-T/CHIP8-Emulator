# Compiler
CC := g++

# Compiler flags
CFLAGS := -Wall -Wextra -O2

# Include and library paths
# Adjust RAYLIB_PATH if your Raylib installation is elsewhere
RAYLIB_PATH := /usr/local
INCLUDES := -I$(RAYLIB_PATH)/include
LDFLAGS := -L$(RAYLIB_PATH)/lib

# Libraries to link against
# Ensure these libraries are available in your LDFLAGS path
LIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Source files
# chip8.cpp contains the emulator logic
# main.cpp contains the Raylib frontend and main loop
SRCS := chip8.cpp main.cpp

# Object files
OBJS := $(SRCS:.cpp=.o)

# Executable name
TARGET := chip8emulator

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS)

# Compilation
# This rule compiles .cpp files to .o object files
%.o: %.cpp Chip8.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
