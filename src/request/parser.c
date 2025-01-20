#include "parser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "request.h"

int count_chars(char* s, char match) {
  int count = 0;
  for (int i = 0; s[i]; ++i) {
    if (s[i] == match) {
      ++count;
    }
  }
  return count;
}

char* parse_method(char* rdp, struct request* req) {
  static const char* ws = " ";
  char* method = rdp;
  rdp += strcspn(rdp, ws);
  if (*rdp) {
    *rdp++ = 0;
  }

  req->method = method;

  return rdp;
}

char* parse_query(char* rdp, struct request* req) {
  static const char* ws = " ";
  static const char* q = "?";
  rdp += strcspn(rdp, q);
  if (*rdp) {
    *rdp++ = 0;
  }
  char* query = rdp;
  rdp += strcspn(rdp, ws);
  if (*rdp) {
    *rdp++ = 0;
  }

  if (strlen(query) > 0) {
    int query_params_count = count_chars(query, '&') + 1;

    struct request_query_param* query_params =
        malloc(query_params_count * sizeof(struct request_query_param));

    for (int i = 0; i < query_params_count; ++i) {
      size_t name_len = strcspn(query, "=");
      query_params[i].name = strndup(query, name_len);

      query += name_len + 1;

      size_t value_len = strcspn(query, "&");
      query_params[i].value = strndup(query, value_len);

      query += value_len + 1;
    }

    req->query = query_params;
    req->query_params_count = query_params_count;
  }

  return rdp;
}

char* parse_path(char* rdp, struct request* req) {
  static const char* ws = " ";
  char* path = rdp;
  rdp += strcspn(rdp, ws);
  if (*rdp) {
    *rdp++ = 0;
  }

  req->path = path;
  parse_query(path, req);

  return rdp;
}

struct request parse_request(char* request_data) {
  struct request req;
  req.query_params_count = 0;
  req.query = NULL;

  char* rdp = request_data;
  rdp = parse_method(rdp, &req);
  rdp = parse_path(rdp, &req);

  return req;
}
