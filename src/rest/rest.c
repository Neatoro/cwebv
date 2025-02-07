#include "rest.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/string_helper.h"

void rest_request_handler(
    rest_server* rest_srv, request* req, struct response* res
) {
  printf("Path: %s, Method: %s\n", req->path, req->method);

  char* method = strlwr(req->method);
  char* path = req->path;
  rest_handler* matched_handler = NULL;
  for (size_t i = 0; i < rest_srv->registered_handlers; ++i) {
    rest_handler handler = rest_srv->handlers[i];
    if (strcmp(method, handler.method) == 0 &&
        strcmp(path, handler.path) == 0) {
      matched_handler = &handler;
      break;
    }
  }

  if (matched_handler == NULL) {
    response_send(res);
  } else {
    matched_handler->handler(req, res);
  }

  free(method);
}

rest_server rest_server_init(http_server* http_srv) {
  rest_server rest_srv;
  rest_srv.http_srv = http_srv;

  rest_srv.registered_handlers = 0;
  rest_srv.allocated_handlers = 0;
  rest_srv.handlers = NULL;

  return rest_srv;
}

void rest_server_free(rest_server* rest_srv) {
  if (rest_srv->handlers != NULL) {
    for (size_t i = 0; i < rest_srv->registered_handlers; ++i) {
      free(rest_srv->handlers[i].method);
    }

    free(rest_srv->handlers);
  }
}

void rest_server_add_handler(
    rest_server* rest_srv, char* method, char* path,
    void (*handler)(request* req, response* res)
) {
  rest_handler rest_hdl;
  rest_hdl.method = strlwr(method);
  rest_hdl.path = path;
  rest_hdl.handler = handler;

  if (rest_srv->allocated_handlers == 0) {
    rest_srv->handlers = calloc(1, sizeof(rest_handler));
    rest_srv->allocated_handlers = 1;
  } else if (rest_srv->allocated_handlers == rest_srv->registered_handlers) {
    rest_srv->handlers = realloc(
        rest_srv->handlers,
        sizeof(rest_handler) * (rest_srv->allocated_handlers * 2)
    );
    rest_srv->allocated_handlers = rest_srv->allocated_handlers * 2;
  }

  memcpy(
      rest_srv->handlers + rest_srv->registered_handlers, &rest_hdl,
      sizeof(rest_handler)
  );

  rest_srv->registered_handlers++;
}
