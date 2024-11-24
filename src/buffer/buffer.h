#ifndef _CWEBV_BUFFER
#define _CWEBV_BUFFER
#include <stdint.h>

#define DEFAULT_SIZE 1024 * sizeof(char)

struct buffer {
  uint32_t used_size;
  uint32_t allocated_size;
  char* data;
};

struct buffer init_buffer();

void free_buffer(struct buffer buf);

void append_data_to_buffer(struct buffer* buf, char data[], int len);
#endif
