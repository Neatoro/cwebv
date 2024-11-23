#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdbool.h>

#define CLOSE_HANDLER(serv) \
void sigint_handler(int signal) { \
  if (signal == SIGINT) { \
    close_server(&serv); \
    exit(0); \
  } \
} \
\
void register_exit_handler() { \
  struct sigaction act; \
\
  bzero(&act, sizeof(act)); \
\
  act.sa_handler = &sigint_handler; \
\
  sigaction(SIGINT, &act, NULL); \
}\

struct server {
  int sock;
  bool error;
  bool closed;
};

void create_http_server(struct server* serv, int port);

void start_server(struct server* serv);

void close_server(struct server* serv);
