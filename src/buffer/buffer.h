#pragma once

#include <stdint.h>

#define DEFAULT_SIZE 1024 * sizeof(char)

typedef struct buffer {
  uint32_t used_size;
  uint32_t allocated_size;
  char* data;
} buffer;

buffer buffer_init();

void buffer_free(buffer* buf);

void buffer_append_data(buffer* buf, char* data, int len);
