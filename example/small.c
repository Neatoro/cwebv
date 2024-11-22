#include "http.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  struct server serv;
  create_http_server(&serv, 8080);
  
  if (serv.error) {
    printf("Server failed to bind\n");
    return -1;
  }

  start_server(&serv);
}
