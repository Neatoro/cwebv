#ifndef _CWEBV_REQUEST
#define _CWEBV_REQUEST
struct request {
  char* method;
  char* path;
  char* query;
};
#endif
