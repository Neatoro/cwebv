#pragma once

#include <signal.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "request.h"
#include "response.h"

#define CLOSE_HANDLER(serv)           \
  void sigint_handler(int signal) {   \
    if (signal == SIGINT) {           \
      close_server(&serv);            \
      exit(0);                        \
    }                                 \
  }                                   \
                                      \
  void register_exit_handler() {      \
    struct sigaction act;             \
    bzero(&act, sizeof(act));         \
    act.sa_handler = &sigint_handler; \
    sigaction(SIGINT, &act, NULL);    \
  }

typedef struct server {
  int sock;
  bool error;
  bool closed;
  void (*handler)(struct request *req, response *res);
} server;

server create_http_server(int port);

void start_server(server *serv);
void close_server(server *serv);

void add_request_handler(
    server *serv, void (*handler)(struct request *req, response *res)
);
