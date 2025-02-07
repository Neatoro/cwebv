CC=gcc
AR=ar
CFLAGS=-I./include -I./lib -g
OBJ_DIR=target/obj
EXAMPLE_OBJ_DIR=target/example/obj
LIB_SOURCES := $(shell find src -name '*.c' -o -name '*.h')
LIB_OBJ := $(shell find src -name '*.c' | sed -e s/src\\///g | sed s/\.c/\.o/)
EXAMPLE_SOURCES := $(shell find example -name '*.c')
INCLUDE_SOURCES := $(shell find include -name '*.h')

TARGET = clean compile

lib/cJSON/cJSON.o:
	make -C ./lib/cJSON static

$(OBJ_DIR)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

target/libhttp.a: $(addprefix $(OBJ_DIR)/,$(LIB_OBJ)) lib/cJSON/cJSON.o
	$(AR) rcs target/libhttp.a $^

$(EXAMPLE_OBJ_DIR)/%.o: example/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

target/example/%: target/libhttp.a $(EXAMPLE_OBJ_DIR)/%.o
	$(CC) -o $@ $^ -L./target -lhttp

example: target/example/small target/example/default_send target/example/rest
compile: target/libhttp.a

includes:
	mkdir -p target/includes/cwebd target/includes/cJSON
	cp lib/cJSON/cJSON.h target/includes/cJSON
	cp -R include/*.h target/includes/cwebd 

format: $(EXAMPLE_SOURCES) $(LIB_SOURCES) $(INCLUDE_SOURCES)
	clang-format -i $^

clean:
	make -C ./lib/cJSON clean
	rm -rf target
