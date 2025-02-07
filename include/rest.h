#pragma once

#include "http.h"

#define REST_CLOSE_HANDLER(rest_srv)      \
  void sigint_handler(int signal) {       \
    if (signal == SIGINT) {               \
      close_server(rest_srv.http_server); \
      rest_server_free(&rest_srv);        \
      exit(0);                            \
    }                                     \
  }                                       \
                                          \
  void register_exit_handler() {          \
    struct sigaction act;                 \
    bzero(&act, sizeof(act));             \
    act.sa_handler = &sigint_handler;     \
    sigaction(SIGINT, &act, NULL);        \
  }

typedef struct rest_handler {
  char* method;
  char* path;

  void (*handler)(struct request* req, response* res);
} rest_handler;

typedef struct rest_server {
  server* http_server;

  int registered_handlers;
  int allocated_handlers;

  rest_handler* handlers;
} rest_server;

rest_server rest_server_init(server* http_server);
void rest_server_free(rest_server* rest_srv);

void rest_request_handler(
    rest_server* rest_srv, struct request* req, struct response* res
);

void rest_server_add_handler(
    rest_server* rest_srv, char* method, char* path,
    void (*handler)(struct request* req, response* res)
);
