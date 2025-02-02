#include "response.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "../utils/string_helper.h"

response response_init(int connection) {
  response res;
  res.header_count = 0;
  res.header = NULL;
  res.connection = connection;
  res.response_status = RESPONSE_STATUS_NOT_FOUND;

  res.body = NULL;

  return res;
}

void response_free(response* res) {
  if (res->header != NULL) {
    free(res->header);
  }
}

void response_add_header(response* res, char* name, char* value) {
  response_header header;
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

bool response_has_header(response* res, char* name) {
  response_header* header = res->header;
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

void response_send(response* res) {
  int connection = res->connection;

  int start_line_length = strlen(res->response_status) + 11;
  char* start_line = malloc(start_line_length);
  snprintf(
      start_line, start_line_length, "HTTP/1.1 %s\n", res->response_status
  );
  send(connection, start_line, strlen(start_line), 0);
  free(start_line);

  if (!response_has_header(res, "content-type")) {
    response_add_header(res, "content-type", "text/plain");
  }

  if (res->body == NULL) {
    res->body = "";
  }

  int body_size = strlen(res->body);
  char* content_length_value = int_to_str(body_size);
  response_add_header(res, "content-length", content_length_value);

  for (int i = 0; i < res->header_count; ++i) {
    response_header header = res->header[i];

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
