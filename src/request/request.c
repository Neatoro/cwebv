#include "request.h"

#include <stdio.h>
#include <stdlib.h>

void free_request(struct request* req) {
  for (int i = 0; i < req->query_params_count; ++i) {
    free(req->query[i].name);
    free(req->query[i].value);
  }
  free(req->query);
}
