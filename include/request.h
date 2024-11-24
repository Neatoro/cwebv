#ifndef _CWEBV_REQUEST
#define _CWEBV_REQUEST
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

void free_request(struct request* req);

struct request_query_param* get_query_param(struct request* req, char* name);
#endif
