#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "http_server.h"
#include "request.h"
#include "response.h"

http_server serv;

CLOSE_HANDLER(serv)

void request_handler(request* req, response* res) {
  printf("Requested Path: %s\n", req->path);
  request_query_param* foo = request_get_query_param(req, "foo");
  if (foo != NULL) {
    printf("Foo Query Param: name(%s), value(%s)\n", foo->name, foo->value);
  }

  response_add_header(res, "X-Message", "Hello World");
  response_add_header(res, "Content-Type", "text/html");
  res->body = "Simple Body\n";

  response_send(res);
}

int main(int argc, char* argv[]) {
  serv = http_server_init(8080);

  if (serv.error) {
    return -1;
  }

  http_server_set_request_handler(&serv, request_handler);

  register_exit_handler();

  http_server_start(&serv);
}
