#include <StringBuilder/stringBuilder.h>
#include <stdio.h>
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
  printf("\nString: %s\n", s);
  s[0] = 'x';
  printf("String with first letter modified: %s\n", s);

  printf("Unmodified string builder:\n");
  SB_print(sb);

  SB_free(sb);
  free(s);
  return 0;
}
