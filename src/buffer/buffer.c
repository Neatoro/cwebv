#include "buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct buffer buffer_init() {
  struct buffer buf;
  buf.used_size = 0;
  buf.allocated_size = DEFAULT_SIZE;

  char* data = malloc(DEFAULT_SIZE);
  memset(data, 0, sizeof(char));

  buf.data = data;

  return buf;
}

void buffer_free(struct buffer buf) { free(buf.data); }

void extend_buffer(struct buffer* buf) {
  int new_size = buf->allocated_size + DEFAULT_SIZE;

  char* new_data = malloc(new_size);
  memset(new_data, 0, sizeof(char));
  strcpy(new_data, buf->data);

  buf->allocated_size = new_size;
  free(buf->data);
  buf->data = new_data;
}

void buffer_append_data(struct buffer* buf, char data[], int len) {
  if (buf->used_size + len > buf->allocated_size) {
    extend_buffer(buf);
  }

  strcat(buf->data, data);

  buf->used_size = strlen(buf->data);
}
