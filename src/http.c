#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int create_http_server(int port) {
  struct sockaddr_in server;

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  memset(&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(port);

  if (bind(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
    return -1;
  }

  return sock;
}

void start_server(int server) {
  struct sockaddr_in client;
  unsigned int len;
  
  listen(server, 10);

  printf("Server ready\n");

  for (;;) {
    len = sizeof(client);
    int connection = accept(server, (struct sockaddr*)&client, &len);

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

int main(int argc, char *argv[]) {
  int server = create_http_server(8080);
  if (server < -1) {
    printf("Server failed to bind\n");
    return -1;
  }

  start_server(server);
}
