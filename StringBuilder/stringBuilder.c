#include "stringBuilder.h"
#include "arrayADT.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void printChar(char* c) {
  char s[3];
  switch (*c) {
  case '\n':
    s[0] = '\\';
    s[1] = 'n';
    s[2] = 0;
    break;
  case '\t':
    s[0] = '\\';
    s[1] = 't';
    s[2] = 0;
    break;
  default:
    s[0] = *c;
    s[1] = 0;
    break;
  }
  printf("'%s'", s);
}

StringBuilder SB_initialize(size_t initialCapacity) {
  return Array_initialize(sizeof(char), initialCapacity, (PrintFn)printChar, NULL);
}

bool SB_push(StringBuilder sb, char c) {
  return Array_push(sb, &c);
}

size_t SB_len(StringBuilder sb) {
  return Array_getLen(sb);
}

char* SB_toString(StringBuilder sb) {
  size_t len = Array_getLen(sb);
  char* s = malloc(len + 1);
  Array_copyVanillaArrayInto(sb, s);
  s[len] = 0;
  return s;
}

void SB_print(StringBuilder sb) {
  Array_print(sb);
}

void SB_free(StringBuilder sb) {
  Array_free(sb);
}
