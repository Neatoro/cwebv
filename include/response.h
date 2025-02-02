#ifndef _CWEBV_RESPONSE
#define _CWEBV_RESPONSE
typedef struct response_header {
  char* name;
  char* value;
} response_header;

#define response_header_size sizeof(response_header)

typedef struct response {
  int header_count;
  int connection;
  response_header* header;

  char* body;
} response;

response response_init(int connection);

void response_add_header(response* res, char* name, char* value);

void response_free(response* res);

void response_send(response* res);
#endif
