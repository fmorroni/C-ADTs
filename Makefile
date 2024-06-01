CC := gcc
INCLUDES_DIR := includes
CCFLAGS := -Wall --pedantic -fsanitize=address -I $(INCLUDES_DIR)
SRC_DIR := src
OBJ_DIR := objs
BIN_DIR := bin
TESTS_DIR := tests
DEPS_DIR := deps

ADTS := $(notdir $(wildcard $(SRC_DIR)/*))

INNER_OBJ_DIRS := $(addprefix $(OBJ_DIR)/, $(ADTS)) $(OBJ_DIR)/$(TESTS_DIR)
INNER_DEPS_DIRS := $(addprefix $(DEPS_DIR)/, $(ADTS)) $(DEPS_DIR)/$(TESTS_DIR) $(DEPS_DIR)/$(BIN_DIR)

SOURCES := $(wildcard $(SRC_DIR)/*/*.c)

ADT_FILES := $(patsubst $(SRC_DIR)/%,%,$(SOURCES))

TEST_SOURCES := $(wildcard $(TESTS_DIR)/*.c)

BINARIES := $(patsubst $(TESTS_DIR)/%.c, $(BIN_DIR)/%, $(TEST_SOURCES))
# BINARY_NAMES := $(patsubst $(TESTS_DIR)/%.c, %, $(TEST_SOURCES))

# SRC_OBJS := $(addprefix $(OBJ_DIR)/, $(ADT_FILES:.c=.o))
# TEST_OBJS := $(addprefix $(OBJ_DIR)/, $(TEST_SOURCES:.c=.o))

DEPS := $(addprefix $(DEPS_DIR)/, $(ADT_FILES:.c=.d)) $(addprefix $(DEPS_DIR)/, $(TEST_SOURCES:.c=.d)) $(addprefix $(DEPS_DIR)/, $(addsuffix .d, $(BINARIES)))

all: $(BINARIES)

# Include dependency files only if the target is not 'clean'
ifneq ($(MAKECMDGOALS),clean)
include $(DEPS)
endif

$(OBJ_DIR): $(INNER_OBJ_DIRS)
	mkdir -p $(OBJ_DIR)
$(INNER_OBJ_DIRS):
	mkdir -p $(addprefix $(OBJ_DIR)/, $(ADTS))
	mkdir -p $(OBJ_DIR)/$(TESTS_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(DEPS_DIR): $(INNER_DEPS_DIRS)
	mkdir -p $(DEPS_DIR)
$(INNER_DEPS_DIRS):
	mkdir -p $(addprefix $(DEPS_DIR)/, $(ADTS))
	mkdir -p $(DEPS_DIR)/$(TESTS_DIR)
	mkdir -p $(DEPS_DIR)/$(BIN_DIR)

$(BIN_DIR)/%: $(OBJ_DIR)/$(TESTS_DIR)/%.o | $(BIN_DIR)
	$(CC) $(CCFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: | $(OBJ_DIR)
	$(CC) $(CCFLAGS) -c $< -o $@

$(OBJ_DIR)/$(TESTS_DIR)/%.o: | $(OBJ_DIR)
	$(CC) $(CCFLAGS) -c $< -o $@

# Need to fix circular dependencies
$(DEPS_DIR)/%.d: $(SRC_DIR)/%.c | $(DEPS_DIR)
	@set -e; \
	$(CC) -MM $(CCFLAGS) $< -MF $@ -MT "$(OBJ_DIR)/$*.o $@"; \
	perl -p -e 's/includes\/(.*?)\.h/$(OBJ_DIR)\/$$1.o/g' $@ >> $@;

# Need to fix circular dependencies
$(DEPS_DIR)/$(TESTS_DIR)/%.d: $(TESTS_DIR)/%.c | $(DEPS_DIR)
	@set -e; \
	$(CC) -MM $(CCFLAGS) $< -MF $@ -MT "$(OBJ_DIR)/$(TESTS_DIR)/$*.o $@"; \
	perl -p -e 's/includes\/(.*?)\.h/$(OBJ_DIR)\/$$1.o/g' $@ >> $@;

$(DEPS_DIR)/$(BIN_DIR)/%.d: $(TESTS_DIR)/%.c | $(DEPS_DIR)
	@set -e; \
	$(CC) -MM $(CCFLAGS) $< -MF $@ -MT "$(BIN_DIR)/$* $@"; \
	perl -pi -e 's/\\\n//g' $@; \
	perl -pi -e 's/includes\/(.*?)\.h/$(OBJ_DIR)\/$$1.o/g' $@; \
	perl -pi -e 's/(\S*\.c)//g' $@;

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(DEPS_DIR)

.PHONY: all clean
