#include "http_server.h"

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "buffer/buffer.h"
#include "request/parser.h"

http_server http_server_init(int port) {
  struct sockaddr_in srvaddr;
  http_server srv;

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  memset(&srvaddr, 0, sizeof(srvaddr));

  srvaddr.sin_family = AF_INET;
  srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  srvaddr.sin_port = htons(port);

  const int option_value = 1;
  const socklen_t option_length = sizeof(option_value);

  setsockopt(
      sock, SOL_SOCKET, SO_REUSEADDR, (void *)&option_value, option_length
  );

  int bind_res = bind(sock, (struct sockaddr *)&srvaddr, sizeof(srvaddr));
  if (bind_res == -1) {
    printf("Failed binding server: %s\n", strerror(errno));
    srv.error = true;
    return srv;
  }

  srv.sock = sock;
  srv.error = false;
  srv.closed = false;

  return srv;
}

void http_server_start(http_server *srv) {
  struct sockaddr_in client;
  unsigned int len;

  listen(srv->sock, 10);

  printf("http_server ready\n");

  while (!srv->closed) {
    len = sizeof(client);
    int connection = accept(srv->sock, (struct sockaddr *)&client, &len);

    if (connection < 0) {
      printf("Failed to accept connection\n");
      continue;
    }

    struct buffer buf = buffer_init();
    int recv_size = 0;
    char *page = malloc(DEFAULT_SIZE);

    do {
      memset(page, 0, DEFAULT_SIZE);

      if ((recv_size = recv(connection, page, DEFAULT_SIZE, 0)) < 0) {
        printf("Failed to read buffer\n");
        break;
      }

      buffer_append_data(&buf, page, recv_size);
    } while (recv_size == DEFAULT_SIZE);

    free(page);

    request req = parse_request(buf.data);
    response res = response_init(connection);
    response_add_header(&res, "Host", "localhost");

    if (srv->handler) {
      srv->handler(&req, &res);
    }

    close(connection);
    buffer_free(buf);
    request_free(&req);
    response_free(&res);
  }
}

void http_server_close(http_server *srv) { close(srv->sock); }

void http_server_set_request_handler(
    http_server *srv, void (*handler)(request *req, response *res)
) {
  srv->handler = handler;
}
