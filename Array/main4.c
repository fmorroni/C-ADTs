#include "arrayADT.h"
#include <stdio.h>
#include <stdlib.h>

void printChar(char* c) {
  printf("'%c'", *c);
}

int main() {
  Array s = Array_initialize(sizeof(char), 10, (PrintFn)printChar, NULL);

  char c;
  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      Array_printInfo(s);
      Array_print(s);
    } else Array_push(s, &c);
  }
  c = 0;
  Array_push(s, &c);

  const char* sv = Array_getVanillaArray(s);
  printf("String: %s\n", sv);

  char* svc = Array_getVanillaArrayCopy(s);
  svc[0] = 'x';
  printf("String: %s\n", svc);

  Array_print(s);

  Array_free(s);
  free(svc);
  return 0;
}
