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

lib/cJSON/libcjson.a:
	make -C ./lib/cJSON static

$(OBJ_DIR)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

target/libcwebv.a: $(addprefix $(OBJ_DIR)/,$(LIB_OBJ))
	$(AR) rcs $@ $^

$(EXAMPLE_OBJ_DIR)/%.o: example/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

target/example/%: target/libcwebv.a lib/cJSON/libcjson.a $(EXAMPLE_OBJ_DIR)/%.o
	$(CC) -o $@ $^ -L./target

example: target/example/small target/example/default_send target/example/rest
compile: target/libcwebv.a lib/cJSON/libcjson.a

includes:
	mkdir -p target/includes/cwebv target/includes/cJSON
	cp lib/cJSON/cJSON.h target/includes/cJSON
	cp -R include/*.h target/includes/cwebv 

release: clean compile includes
	cp lib/cJSON/libcjson.a target
	rm -rf target/obj

format: $(EXAMPLE_SOURCES) $(LIB_SOURCES) $(INCLUDE_SOURCES)
	clang-format -i $^

clean:
	make -C ./lib/cJSON clean
	rm -rf target
