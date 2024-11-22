#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>

struct server serv;

CLOSE_HANDLER(serv)

int main(int argc, char *argv[]) {
  create_http_server(&serv, 8080);
  
  if (serv.error) {
    return -1;
  }

  register_exit_handler();

  start_server(&serv);
}
