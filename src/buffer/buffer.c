#include "buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/math.h"

buffer buffer_init() {
  buffer buf;
  buf.used_size = 0;
  buf.allocated_size = DEFAULT_SIZE;

  char* data = malloc(DEFAULT_SIZE);
  if (data == NULL) {
    fprintf(stderr, "Memory reallocation failed\n");
    exit(EXIT_FAILURE);
  }

  memset(data, 0, DEFAULT_SIZE);

  buf.data = data;

  return buf;
}

void buffer_free(buffer* buf) { free(buf->data); }

void extend_buffer(buffer* buf, int required_additional_size) {
  int new_size = MATH_MAX(
      buf->allocated_size * 2, buf->allocated_size + required_additional_size
  );

  char* new_data = realloc(buf->data, new_size);
  if (new_data == NULL) {
    fprintf(stderr, "Memory reallocation failed\n");
    exit(EXIT_FAILURE);
  }

  buf->data = new_data;
  buf->allocated_size = new_size;
}

void buffer_append_data(buffer* buf, char* data, int len) {
  if (buf->used_size + len > buf->allocated_size) {
    extend_buffer(buf, len);
  }

  for (int i = 0; i < len; ++i) {
    buf->data[buf->used_size + i] = data[i];
  }

  buf->used_size = buf->used_size + len;
}
