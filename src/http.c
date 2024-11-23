#include "http.h"

#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void create_http_server(struct server *serv, int port) {
  struct sockaddr_in servaddr;

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  const int option_value = 1;
  const socklen_t option_length = sizeof(option_value);

  int rtn = setsockopt(
      sock, SOL_SOCKET, SO_REUSEADDR, (void *)&option_value, option_length
  );

  assert(rtn == 0);

  int bind_res = bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
  if (bind_res == -1) {
    printf("Failed binding server: %s\n", strerror(errno));
    serv->error = true;
    return;
  }

  serv->sock = sock;
  serv->error = false;
  serv->closed = false;
}

void start_server(struct server *serv) {
  struct sockaddr_in client;
  unsigned int len;

  listen(serv->sock, 10);

  printf("Server ready\n");

  while (!serv->closed) {
    len = sizeof(client);
    int connection = accept(serv->sock, (struct sockaddr *)&client, &len);

    if (connection < 0) {
      printf("Failed to accept connection\n");
      continue;
    }

    char buffer[1025];
    snprintf(buffer, sizeof(buffer), "Hello from Server\n");
    write(connection, buffer, strlen(buffer));

    close(connection);
  }
}

void close_server(struct server *serv) { close(serv->sock); }
