#ifndef _CWEBV_HTTP
#define _CWEBV_HTTP
#include <signal.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>

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

struct server {
  int sock;
  bool error;
  bool closed;
};

struct server create_http_server(int port);

void start_server(struct server *serv);

void close_server(struct server *serv);
#endif
