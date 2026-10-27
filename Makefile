CC       := gcc
CPPFLAGS := -Iinclude -D_POSIX_C_SOURCE=200809L -MMD -MP
CFLAGS   := -std=c11 -Wall -Wextra -Werror
LDFLAGS  :=

MODE ?= dev
ifeq ($(MODE),debug)
    CFLAGS  += -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer
    LDFLAGS += -fsanitize=address,undefined
else ifeq ($(MODE),release)
    CFLAGS  += -O2 -DNDEBUG
else ifeq ($(MODE),dev)
    CFLAGS  += -g -O0
else
    $(error unknown MODE '$(MODE)', use dev, debug or release)
endif

SRC_DIR   := src
BUILD_DIR := build/$(MODE)
TARGET    := $(BUILD_DIR)/mini-shell

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all debug release test memcheck clean

all: $(TARGET)

debug:
	$(MAKE) MODE=debug

release:
	$(MAKE) MODE=release

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

test: $(TARGET)
	@RUNNER="$(RUNNER)" sh tests/run.sh $(TARGET)

# Valgrind can't run ASan binaries, so this always uses the dev build
memcheck:
	$(MAKE) test MODE=dev RUNNER="valgrind -q --leak-check=full --show-leak-kinds=definite,indirect,possible --errors-for-leak-kinds=definite,indirect,possible --error-exitcode=99"

clean:
	rm -rf build

-include $(DEPS)
