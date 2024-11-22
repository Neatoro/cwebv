CC=gcc
AR=ar
CFLAGS=-I./include
OBJ_DIR=target/obj
EXAMPLE_OBJ_DIR=target/example/obj

TARGET = clean compile

$(shell mkdir -p $(OBJ_DIR) $(EXAMPLE_OBJ_DIR))

$(OBJ_DIR)/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

target/libhttp.a: $(OBJ_DIR)/http.o
	$(AR) rcs target/libhttp.a $(OBJ_DIR)/http.o

$(EXAMPLE_OBJ_DIR)/%.o: example/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

target/example/%: target/libhttp.a $(EXAMPLE_OBJ_DIR)/%.o
	$(CC) -o $@ $^ -L./target -lhttp

example: target/example/small
compile: target/libhttp.a

clean:
	rm -rf target
