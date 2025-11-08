# Makefile for toy_rasp
# Mirrors compile.sh (debug build with -g -O0)

CC := gcc
CFLAGS_DEBUG := -g -O0 -Wall -Wextra -std=c11
CFLAGS_RELEASE := -O2 -Wall -Wextra -std=c11
LDFLAGS :=

TARGET := toy_rasp

SRCS := main.c \
	 hashtable/hashtable.c \
	 malloc_utils/malloc_utils.c \
	 rasp_executor/rasp_context.c \
	 rasp_instruction/rasp_instruction.c \
	 rasp_program/rasp_program.c

OBJS := $(SRCS:.c=.o)

.PHONY: all debug release clean run

all: debug

# Default debug build (matches compile.sh)
debug: CFLAGS := $(CFLAGS_DEBUG)
debug: $(TARGET)

release: CFLAGS := $(CFLAGS_RELEASE)
release: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Generic rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: debug
	./$(TARGET) $(ARGS)

# Convenience alias to match compile.sh behavior
compile.sh: $(TARGET)
	@echo "Built $(TARGET)"
