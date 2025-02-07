#pragma once

typedef struct request_query_param {
  char* name;
  char* value;
} request_query_param;

typedef struct request {
  char* method;
  char* path;

  int query_params_count;
  request_query_param* query;
} request;

void request_free(request* req);

request_query_param* request_get_query_param(request* req, char* name);
