#include "response.h"

#include <stdlib.h>

void free_response(struct response* res) {
  for (int i = 0; i < res->header_count; ++i) {
    free(res->header[i].name);
    free(res->header[i].value);
  }
  
  if (res->header != NULL) {
    free(res->header);
  }
}
