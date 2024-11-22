#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>

struct server serv;

void sigint_handler(int signal) {
  if (signal == SIGINT) {
    close_server(&serv);
    exit(0);
  }
}

void register_exit_handler() {
  struct sigaction act;

  bzero(&act, sizeof(act));

  act.sa_handler = &sigint_handler;

  sigaction(SIGINT, &act, NULL);
}


int main(int argc, char *argv[]) {
  create_http_server(&serv, 8080);
  
  if (serv.error) {
    return -1;
  }

  register_exit_handler();

  start_server(&serv);
}
