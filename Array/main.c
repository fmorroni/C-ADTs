#include "arrayADT.h"
#include <stdio.h>

typedef struct {
  int a;
  char b[20];
} Test;

void printTest(Test* t) {
  printf("{ a: %d, b: \"%s\" }", t->a, t->b);
}

void printInt(int* i) {
  printf("%d", *i);
}

void printChar(char* c) {
  printf("%c", *c);
}

void mapIntToTest(Test* mappedEle, int* x) {
  mappedEle->a = *x * 2;
  sprintf(mappedEle->b, "Test %d", *x);
}

void mapTestToInt(int* mappedEle, Test* x) {
  *mappedEle = x->a;
}

int main() {
  Array testArr = Array_initialize(sizeof(Test), 2, (PrintFn)printTest, NULL);
  Test t = {0, "Test 0"};
  Array_push(testArr, &t);
  Array_push(testArr, &t);
  for (int i = 1; i < 10; ++i) {
    t.a = i;
    sprintf(t.b, "Test %d", i);
    Array_push(testArr, &t);
  }

  Array_printInfo(testArr);
  Array_print(testArr);
  puts("");

  Array intArr = Array_initialize(sizeof(int), 5, (PrintFn)printInt, NULL);
  int x = 3;
  Array_push(intArr, &x);
  Array_push(intArr, &x);
  Array_push(intArr, &x);
  x = 5;
  Array_push(intArr, &x);
  Array_push(intArr, &x);
  x = 1;
  Array_push(intArr, &x);
  Array_push(intArr, &x);
  for (int i = 0; i < 100; ++i) {
    Array_push(intArr, &i);
  }

  Array_printInfo(intArr);
  Array_print(intArr);
  puts("");

  Array mappedArr = Array_map(intArr, (MapFn)mapIntToTest, sizeof(Test), (PrintFn)printTest, NULL);

  Array_printInfo(mappedArr);
  Array_print(mappedArr);
  puts("");

  printf("mappedArr[4]: ");
  printTest(Array_getEle(mappedArr, 4));
  printf("\n\n");

  char a[] = {'a', 'b', 'c'};
  Array aArr = Array_fromVanillaArray(a, 3, sizeof(char), (PrintFn)printChar, NULL);
  char c = 'x';
  Array_push(aArr, &c);
  Array_push(aArr, &c);

  Array_printInfo(aArr);
  Array_print(aArr);

  Test tl[] = {{1, "asdf"}, {1234, "qwer"}, {999, "hola carola"}};
  Array tlArr = Array_fromVanillaArray(tl, sizeof(tl) / sizeof(tl[0]), sizeof(Test), (PrintFn)printTest, NULL);

  Array_push(tlArr, tl + 1);
  Array_push(tlArr, &t);

  Array_printInfo(tlArr);
  Array_print(tlArr);

  Array mappedArr2 = Array_map(tlArr, (MapFn)mapTestToInt, sizeof(int), (PrintFn)printInt, NULL);

  Array_printInfo(mappedArr2);
  Array_print(mappedArr2);
  printf("\n\n");

  Array_free(testArr);
  Array_free(intArr);
  Array_free(mappedArr);
  Array_free(aArr);
  Array_free(tlArr);
  Array_free(mappedArr2);
  return 0;
}
