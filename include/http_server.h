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
      http_server_close(&serv);       \
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

typedef struct http_server {
  int sock;
  bool error;
  bool closed;
  void (*handler)(request *req, response *res);
} http_server;

http_server http_server_init(int port);

void http_server_start(http_server *srv);
void http_server_close(http_server *srv);

void http_server_set_request_handler(
    http_server *srv, void (*handler)(request *req, response *res)
);
