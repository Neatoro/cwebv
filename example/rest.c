#include "rest.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "http.h"
#include "request.h"
#include "response.h"

rest_server rest_srv;

REST_CLOSE_HANDLER(rest_srv)

void request_handler(struct request* req, struct response* res) {
  rest_request_handler(&rest_srv, req, res);
}

void post_root(struct request* req, response* res) {
  printf("Hello World from POST\n");
  res->response_status = RESPONSE_STATUS_CREATED;
  response_send(res);
}

void get_message(struct request* req, response* res) {
  printf("Hello World from GET\n");
  res->response_status = RESPONSE_STATUS_OK;
  response_send(res);
}

int main(int argc, char* argv[]) {
  server srv = create_http_server(8080);

  if (srv.error) {
    return -1;
  }

  register_exit_handler();

  add_request_handler(&srv, request_handler);

  rest_srv = rest_server_init(&srv);

  rest_server_add_handler(&rest_srv, "get", "/message", get_message);
  rest_server_add_handler(&rest_srv, "post", "/", post_root);

  start_server(&srv);
}
