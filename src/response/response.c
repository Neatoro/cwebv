#include "response.h"

#include <stdlib.h>

void response_free(struct response* res) {
  for (int i = 0; i < res->header_count; ++i) {
    free(res->header[i].name);
    free(res->header[i].value);
  }

  if (res->header != NULL) {
    free(res->header);
  }
}
