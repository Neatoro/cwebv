#ifndef _CWEBV_RESPONSE
#define _CWEBV_RESPONSE
struct response_header {
  char* name;
  char* value;
};

struct response {
  int header_count;
  struct response_header* header;

  char* body;
};

void free_response(struct response* req);
#endif
