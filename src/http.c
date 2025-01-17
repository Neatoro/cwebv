#include "http.h"

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "buffer/buffer.h"
#include "request/parser.h"

struct response create_response(int connection) {
  struct response res;
  res.header_count = 0;
  res.header = NULL;
  res.connection = connection;
  return res;
}

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

    struct buffer buf = buffer_init();
    int recv_size = 0;
    char page[DEFAULT_SIZE];

    do {
      memset(&page, 0, sizeof(page));

      if ((recv_size = recv(connection, page, DEFAULT_SIZE, 0)) < 0) {
        printf("Failed to read buffer\n");
        break;
      }

      buffer_append_data(&buf, page, recv_size);
    } while (recv_size == DEFAULT_SIZE);

    struct request req = parse_request(buf.data);
    struct response res = create_response(connection);
    response_add_header(&res, "Host", "localhost");

    if (serv->handler) {
      serv->handler(&req, &res);
    }

    close(connection);
    buffer_free(buf);
    request_free(&req);
    response_free(&res);
  }
}

void close_server(struct server *serv) { close(serv->sock); }

void add_request_handler(
    struct server *serv,
    void (*handler)(struct request *req, struct response *res)
) {
  serv->handler = handler;
}
