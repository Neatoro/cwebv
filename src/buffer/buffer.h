#pragma once

#include <stdint.h>

#define DEFAULT_SIZE 1024 * sizeof(char)

struct buffer {
  uint32_t used_size;
  uint32_t allocated_size;
  char* data;
};

struct buffer buffer_init();

void buffer_free(struct buffer buf);

void buffer_append_data(struct buffer* buf, char* data, int len);
