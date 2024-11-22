#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>

struct server {
  int sock;
  bool error;
  bool closed;
};

void create_http_server(struct server* serv, int port);

void start_server(struct server* serv);

void close_server(struct server* serv);
