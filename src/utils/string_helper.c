#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* strlwr(char* original) {
  char* lower = strdup(original);

  for (int i = 0; original[i]; ++i) {
    lower[i] = tolower(original[i]);
  }

  return lower;
}

char* int_to_str(int i) {
  int length = snprintf(NULL, 0, "%d", i);
  char* str = malloc(length + 1);
  snprintf(str, length + 1, "%d", i);
  return str;
}
