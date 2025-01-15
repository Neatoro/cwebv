#include "request.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void request_free(struct request* req) {
  for (int i = 0; i < req->query_params_count; ++i) {
    free(req->query[i].name);
    free(req->query[i].value);
  }
  free(req->query);
}

struct request_query_param* get_query_param(struct request* req, char* name) {
  for (int i = 0; i < req->query_params_count; ++i) {
    if (strcmp(req->query[i].name, name) == 0) {
      return &(req->query[i]);
    }
  }
  return NULL;
}
