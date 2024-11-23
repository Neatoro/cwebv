CC=gcc
AR=ar
CFLAGS=-I./include
OBJ_DIR=target/obj
EXAMPLE_OBJ_DIR=target/example/obj
LIB_SOURCES := $(shell find src -name '*.c' -o -name '*.h')
EXAMPLE_SOURCES := $(shell find example -name '*.c')
INCLUDE_SOURCES := $(shell find include -name '*.h')

TARGET = clean compile

$(OBJ_DIR)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

target/libhttp.a: $(OBJ_DIR)/http.o $(OBJ_DIR)/buffer/buffer.o
	$(AR) rcs target/libhttp.a $^

$(EXAMPLE_OBJ_DIR)/%.o: example/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

target/example/%: target/libhttp.a $(EXAMPLE_OBJ_DIR)/%.o
	$(CC) -o $@ $^ -L./target -lhttp

example: target/example/small
compile: target/libhttp.a

format: $(EXAMPLE_SOURCES) $(LIB_SOURCES) $(INCLUDE_SOURCES)
	clang-format -i $^

clean:
	rm -rf target
