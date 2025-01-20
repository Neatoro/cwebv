#include "response.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "../utils/string_helper.h"

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
    res->header =
        realloc(res->header, response_header_size * (res->header_count + 1));
  } else {
    res->header = calloc(1, response_header_size);
  }

  memcpy(res->header + res->header_count, &header, sizeof(header));

  res->header_count++;
}

bool response_has_header(struct response* res, char* name) {
  struct response_header* header = res->header;
  for (int i = 0; i < res->header_count; ++i) {
    char* header_name = strlwr(header->name);
    if (strcmp(header_name, name) == 0) {
      free(header_name);
      return true;
    }

    free(header_name);
    ++header;
  }
  return false;
}

void response_send(struct response* res) {
  int connection = res->connection;

  char* start_line = "HTTP/1.1 200 OK\n";
  send(connection, start_line, strlen(start_line), 0);

  if (!response_has_header(res, "content-type")) {
    response_add_header(res, "content-type", "text/plain");
  }

  int body_size = strlen(res->body);
  char* content_length_value = int_to_str(body_size);
  response_add_header(res, "content-length", content_length_value);

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
  send(connection, res->body, strlen(res->body), 0);

  free(content_length_value);
}
