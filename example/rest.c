#include "rest.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "http_server.h"
#include "request.h"
#include "response.h"

rest_server rest_srv;

REST_CLOSE_HANDLER(rest_srv)

void request_handler(request* req, struct response* res) {
  rest_request_handler(&rest_srv, req, res);
}

void post_root(request* req, response* res) {
  printf("Hello World from POST\n");
  res->response_status = RESPONSE_STATUS_CREATED;
  response_send(res);
}

void get_message(request* req, response* res) {
  printf("Hello World from GET\n");
  res->response_status = RESPONSE_STATUS_OK;
  response_send(res);
}

int main(int argc, char* argv[]) {
  http_server http_srv = http_server_init(8080);

  if (http_srv.error) {
    return -1;
  }

  register_exit_handler();

  http_server_set_request_handler(&http_srv, request_handler);

  rest_srv = rest_server_init(&http_srv);

  rest_server_add_handler(&rest_srv, "get", "/message", get_message);
  rest_server_add_handler(&rest_srv, "post", "/", post_root);

  http_server_start(&http_srv);
}
