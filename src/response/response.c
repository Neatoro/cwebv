#include "response.h"

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

void response_free(struct response* res) {
  if (res->header != NULL) {
    free(res->header);
  }
}

void response_add_header(struct response* res, char* name, char* value) {
  struct response_header header;
  header.name = name;
  header.value = value;

  if (res->header != NULL) {
    res->header = realloc(
        res->header, sizeof(struct response_header) * (res->header_count + 1)
    );
  } else {
    res->header = calloc(1, sizeof(struct response_header));
  }

  memcpy(
      res->header + (sizeof(struct response_header) * res->header_count),
      &header, sizeof(header)
  );

  res->header_count++;
}

void response_send(struct response* res) {
  int connection = res->connection;

  char* start_line = "HTTP/1.1 200 OK\n";
  send(connection, start_line, strlen(start_line), 0);

  char* data = "OK\n";

  for (int i = 0; i < res->header_count; ++i) {
    struct response_header header = res->header[i];
    int header_line_size = strlen(header.name) + strlen(header.value) + 4;
    char header_line[header_line_size];
    strcpy(header_line, header.name);
    strcat(header_line, ": ");
    strcat(header_line, header.value);
    strcat(header_line, "\r\n");

    send(connection, header_line, header_line_size, 0);
  }

  send(connection, "\n", 1, 0);
  send(connection, data, strlen(data), 0);
}
