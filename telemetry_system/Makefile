# Compiler and Flags
CC = gcc

# Define the path to your FreeRTOS folder
FREERTOS_DIR = /home/vamsi/my_rtos_project/FreeRTOS/FreeRTOS
FREERTOS_SRC = $(FREERTOS_DIR)/Source

# Include Paths
# 1. Your local headers
# 2. FreeRTOS Core headers
# 3. FreeRTOS POSIX Port headers
CFLAGS = -Wall -Wextra -g \
         -I./include \
         -I$(FREERTOS_SRC)/include \
         -I$(FREERTOS_SRC)/portable/ThirdParty/GCC/Posix

# Target binary name
TARGET = telemetry_app

# Folders
SRCDIR = src
DRVDIR = drivers
OBJDIR = obj

# 1. Collect your project sources (Local)
LOCAL_SOURCES = $(wildcard $(SRCDIR)/*.c) $(wildcard $(DRVDIR)/*.c)

# 2. Define the specific FreeRTOS Kernel sources needed
RTOS_SOURCES = $(FREERTOS_SRC)/tasks.c \
               $(FREERTOS_SRC)/list.c \
               $(FREERTOS_SRC)/queue.c \
               $(FREERTOS_SRC)/timers.c \
               $(FREERTOS_SRC)/event_groups.c \
               $(FREERTOS_SRC)/portable/ThirdParty/GCC/Posix/port.c \
               $(FREERTOS_SRC)/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c \
               $(FREERTOS_SRC)/portable/MemMang/heap_3.c

# Combine all sources into one list
ALL_SOURCES = $(LOCAL_SOURCES) $(RTOS_SOURCES)

# 3. Generate the list of object files in the obj/ directory
# This takes the filename of every .c file and puts it in the obj/ folder as .o
OBJECTS = $(addprefix $(OBJDIR)/, $(notdir $(ALL_SOURCES:.c=.o)))

# 4. Tell 'make' where to search for the .c files (vpath)
vpath %.c $(SRCDIR) $(DRVDIR) $(FREERTOS_SRC) \
          $(FREERTOS_SRC)/portable/ThirdParty/GCC/Posix \
          $(FREERTOS_SRC)/portable/ThirdParty/GCC/Posix/utils \
          $(FREERTOS_SRC)/portable/MemMang

# --- Rules ---

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

# Link the final application
# Note: -lpthread and -lrt are required for the Linux Posix Port
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) -lpthread -lrt -lm

# Compile rule for all object files
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean