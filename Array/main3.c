#include "arrayADT.h"
#include <stdio.h>

void printInt(int* i) {
  printf("%d", *i);
}

void doubleMap(int* mappedEle, int* ele) {
  *mappedEle = *ele * 2;
}

int main() {
  int x[] = {0, 1, 2, 3, 4, 9, 28};
  Array a = Array_fromVanillaArray(x, sizeof(x) / sizeof(x[0]), sizeof(int), (PrintFn)printInt, NULL);
  Array b = Array_map(a, (MapFn)doubleMap, sizeof(int), (PrintFn)printInt, NULL);

  printf("a: ");
  Array_printInfo(a);
  Array_print(a);
  printf("b: ");
  Array_printInfo(b);
  Array_print(b);

  Array_concat(a, b);
  printf("a: ");
  Array_printInfo(a);
  Array_print(a);

  Array_free(a);
  Array_free(b);
  return 0;
}
