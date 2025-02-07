#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "http_server.h"
#include "request.h"
#include "response.h"

server serv;

CLOSE_HANDLER(serv)

void request_handler(struct request* req, response* res) { response_send(res); }

int main(int argc, char* argv[]) {
  serv = http_server_init(8080);

  if (serv.error) {
    return -1;
  }

  http_server_set_request_handler(&serv, request_handler);

  register_exit_handler();

  http_server_start(&serv);
}
