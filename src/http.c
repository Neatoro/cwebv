#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void create_http_server(struct server* serv, int port) {
  struct sockaddr_in servaddr;

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  if (bind(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
    serv->error = true;
    return;
  }

  serv->sock = sock;
  serv->error = false;
}

void start_server(struct server* serv) {
  struct sockaddr_in client;
  unsigned int len;
  
  listen(serv->sock, 10);

  printf("Server ready\n");

  for (;;) {
    len = sizeof(client);
    int connection = accept(serv->sock, (struct sockaddr*)&client, &len);

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
