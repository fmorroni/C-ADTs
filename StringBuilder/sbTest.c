#include "stringBuilder.h"
#include <stdlib.h>

int main() {
  StringBuilder sb = SB_initialize(10);

  char c;
  while ((c = getchar()) != EOF) {
    if (c == '\n') SB_print(sb);
    SB_push(sb, c);
  }
  c = 0;
  SB_push(sb, c);

  char* s = SB_toString(sb);
  s[0] = 'x';
  printf("String: %s\n", s);

  SB_print(sb);

  SB_free(sb);
  free(s);
  return 0;
}
