#include "parser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  printf("%s\n", rdp);

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

  req->query = query;

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

  char* rdp = request_data;
  rdp = parse_method(rdp, &req);
  rdp = parse_path(rdp, &req);

  return req;
}
