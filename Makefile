CC=gcc
CFLAGS=-I./include
OBJ_DIR=target/obj

$(OBJ_DIR)/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

target:
	mkdir -p $(OBJ_DIR)

compile: target $(OBJ_DIR)/http.o
	$(CC) -o target/http $(OBJ_DIR)/http.o

clean:
	rm -rf target
