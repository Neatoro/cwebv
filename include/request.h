#pragma once

struct request_query_param {
  char* name;
  char* value;
};

struct request {
  char* method;
  char* path;

  int query_params_count;
  struct request_query_param* query;
};

void request_free(struct request* req);

struct request_query_param* request_get_query_param(
    struct request* req, char* name
);
