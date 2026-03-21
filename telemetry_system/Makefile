# Compiler and Flags
CC = gcc
# -I./include tells the compiler to look for .h files there
CFLAGS = -Wall -Wextra -I./include -g

# Target binary name
TARGET = telemetry_app

# Folders
SRCDIR = src
DRVDIR = drivers
OBJDIR = obj

# Use this to find all .c files in src and drivers
SOURCES = $(wildcard $(SRCDIR)/*.c) $(wildcard $(DRVDIR)/*.c)

# This line converts those .c paths into obj/*.o names
# The 'notdir' can sometimes cause duplicates if you have the same filename in two folders
OBJECTS = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(SOURCES)))

# Search paths for the compiler to find the .c files for the objects
vpath %.c $(SRCDIR) $(DRVDIR)

# Default rule
all: $(OBJDIR) $(TARGET)

# Create the object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Link the object files into the final executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Compile .c files into .o files
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean