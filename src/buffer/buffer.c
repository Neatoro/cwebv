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
  int new_size = buf->allocated_size * 2;
  buf->allocated_size = new_size;
  buf->data = realloc(buf->data, new_size);
}

void buffer_append_data(struct buffer* buf, char* data, int len) {
  if (buf->used_size + len > buf->allocated_size) {
    extend_buffer(buf);
  }

  for (int i = 0; i < len; ++i) {
    buf->data[buf->used_size + i] = data[i];
  }

  buf->used_size = buf->used_size + len;
}
