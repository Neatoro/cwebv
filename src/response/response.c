#include "response.h"

#include <stdlib.h>
#include <string.h>

void response_free(struct response* res) {
  if (res->header != NULL) {
    free(res->header);
  }
}

void response_add_header(struct response* res, char* name, char* value) {
  struct response_header header;
  header.name = name;
  header.value = value;

  if (res->header != NULL) {
    res->header = realloc(
        res->header, sizeof(struct response_header) * (res->header_count + 1)
    );
  } else {
    res->header = calloc(1, sizeof(struct response_header));
  }

  memcpy(
      res->header + (sizeof(struct response_header) * res->header_count),
      &header, sizeof(header)
  );

  res->header_count++;
}
