#ifndef _CWEBV_RESPONSE
#define _CWEBV_RESPONSE
struct response_header {
  char* name;
  char* value;
};

struct response {
  int header_count;
  int connection;
  struct response_header* header;

  char* body;
};

void response_add_header(struct response* res, char* name, char* value);

void response_free(struct response* res);

void response_send(struct response* res);
#endif
