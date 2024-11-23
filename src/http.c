#include "http.h"

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "buffer/buffer.h"
#include "request.h"
#include "request/parser.h"

struct server create_http_server(int port) {
  struct sockaddr_in servaddr;
  struct server serv;

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  const int option_value = 1;
  const socklen_t option_length = sizeof(option_value);

  setsockopt(
      sock, SOL_SOCKET, SO_REUSEADDR, (void *)&option_value, option_length
  );

  int bind_res = bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
  if (bind_res == -1) {
    printf("Failed binding server: %s\n", strerror(errno));
    serv.error = true;
    return serv;
  }

  serv.sock = sock;
  serv.error = false;
  serv.closed = false;

  return serv;
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

    struct buffer buf = init_buffer();
    int recv_size = 0;
    char page[DEFAULT_SIZE];

    do {
      memset(&page, 0, sizeof(page));

      if ((recv_size = recv(connection, page, DEFAULT_SIZE, 0)) < 0) {
        printf("Failed to read buffer\n");
        break;
      }

      append_data_to_buffer(&buf, page, recv_size);
    } while (recv_size == DEFAULT_SIZE);

    struct request req = parse_request(buf.data);

    close(connection);
    free_buffer(buf);
  }
}

void close_server(struct server *serv) { close(serv->sock); }
