#pragma once

#define RESPONSE_STATUS_OK "200 OK"
#define RESPONSE_STATUS_CREATED "201 Created"

#define RESPONSE_STATUS_NOT_FOUND "404 Not Found"

typedef struct response_header {
  char* name;
  char* value;
} response_header;

#define response_header_size sizeof(response_header)

typedef struct response {
  int header_count;
  int connection;
  response_header* header;
  char* response_status;

  char* body;
} response;

response response_init(int connection);

void response_add_header(response* res, char* name, char* value);

void response_free(response* res);

void response_send(response* res);
