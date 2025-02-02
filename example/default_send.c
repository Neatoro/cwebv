#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "http.h"
#include "request.h"
#include "response.h"

server serv;

CLOSE_HANDLER(serv)

void request_handler(struct request* req, response* res) { response_send(res); }

int main(int argc, char* argv[]) {
  serv = create_http_server(8080);

  if (serv.error) {
    return -1;
  }

  add_request_handler(&serv, request_handler);

  register_exit_handler();

  start_server(&serv);
}
