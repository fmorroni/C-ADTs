#include <Array/array.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ArrayCDT {
  // Use uint8_t* instead of void* because void* can't be used in arithmetic operations.
  uint8_t* array;
  size_t elementSize;
  size_t capacity;
  size_t length;
  PrintFn printFn;
  FreeEleFn freeEleFn;
} ArrayCDT;

void copynEleAt(Array a, size_t idx, const void* eleArr, size_t n);
void copyEleAt(Array a, size_t idx, const void* ele);
bool growTo(Array a, size_t newCapacity);
bool growBy(Array a, size_t extraCapacity);

void* Array_initialize(size_t elementSize, size_t initialCapacity, PrintFn printFn, FreeEleFn freeEleFn) {
  if (elementSize == 0) {
    errno = EINVAL;
    return NULL;
  }
  ArrayCDT* a = malloc(sizeof(ArrayCDT));
  if (a == NULL) return NULL;
  a->capacity = initialCapacity ? initialCapacity : 1;
  a->array = malloc(a->capacity * elementSize);
  if (a->array == NULL) {
    free(a);
    return NULL;
  }
  a->length = 0;
  a->elementSize = elementSize;
  a->printFn = printFn;
  a->freeEleFn = freeEleFn;
  return a;
}

void Array_free(Array a) {
  if (a->freeEleFn != NULL) {
    for (int i = 0; i < a->length; ++i) a->freeEleFn(Array_getEle(a, i));
  }
  free(a->array);
  free(a);
}

bool Array_push(Array a, const void* ele) {
  if (ele == NULL) {
    errno = EINVAL;
    return false;
  }
  if (a->length >= a->capacity) {
    if (!growBy(a, a->capacity)) return false;
  }
  copyEleAt(a, a->length, ele);
  ++a->length;
  return true;
}

size_t Array_getLen(Array a) {
  return a->length;
}

void* Array_getEle(Array a, size_t idx) {
  if (idx >= a->length) {
    errno = EINVAL;
    return NULL;
  }
  return a->array + idx * a->elementSize;
}

bool Array_setEle(Array a, size_t idx, void* ele) {
  if (idx >= a->length) {
    errno = EINVAL;
    return false;
  }
  copyEleAt(a, idx, ele);
  return true;
}

void Array_print(Array a) {
  if (a->printFn == NULL) return;
  printf("[ ");
  for (int i = 0; i < Array_getLen(a); ++i) {
    a->printFn(Array_getEle(a, i));
    if (i < Array_getLen(a) - 1) printf(", ");
  }
  printf(" ]\n");
}

void Array_printInfo(Array a) {
  printf("{ \n");
  printf(
    "  length: %li\n"
    "  elementSize: %li\n"
    "  capacity: %li\n"
    "  array: %p\n",
    a->length, a->elementSize, a->capacity, a->array
  );
  printf("}\n");
}

Array Array_map(Array a, MapFn mapFn, size_t newElemSize, PrintFn newTypePrintFn, FreeEleFn freeFn) {
  Array mapped = Array_initialize(newElemSize, a->length, newTypePrintFn, freeFn);
  for (int i = 0; i < a->length; ++i) {
    void* mappedEle = malloc(newElemSize);
    mapFn(mappedEle, Array_getEle(a, i), i);
    Array_push(mapped, mappedEle);
    free(mappedEle);
  }
  return mapped;
}

Array Array_fromVanillaArray(const void* array, size_t length, size_t elementSize, PrintFn printFn, FreeEleFn freeFn) {
  Array a = Array_initialize(elementSize, length, printFn, freeFn);
  if (a == NULL) return NULL;
  a->length = length;
  copynEleAt(a, 0, array, length);
  return a;
}

bool Array_concat(Array dst, Array src) {
  if (dst->elementSize != src->elementSize) {
    errno = EINVAL;
    return false;
  }
  size_t neededCapacity = dst->length + src->length;
  if (dst->capacity < neededCapacity) {
    if (!growTo(dst, neededCapacity)) return false;
  }
  copynEleAt(dst, dst->length, src->array, src->length);
  dst->length += src->length;
  return true;
}

const void* Array_getVanillaArray(Array a) {
  return a->array;
}

void* Array_copyVanillaArrayInto(Array a, void* array) {
  memcpy(array, a->array, a->length * a->elementSize);
  return array;
}

void* Array_getVanillaArrayCopy(Array a) {
  void* array = malloc(a->length * a->elementSize);
  Array_copyVanillaArrayInto(a, array);
  return array;
}

// char* Array_toString(Array a) {
//   if (a->printFn == NULL) return NULL;
//   printf("[ ");
//   for (int i = 0; i < Array_getLen(a); ++i) {
//     a->printFn(Array_getEle(a, i));
//     if (i < Array_getLen(a) - 1) printf(", ");
//   }
//   printf(" ]\n");
// }

void copynEleAt(Array a, size_t idx, const void* eleArr, size_t n) {
  memcpy(a->array + a->elementSize * idx, eleArr, a->elementSize * n);
}

void copyEleAt(Array a, size_t idx, const void* ele) {
  copynEleAt(a, idx, ele, 1);
}

bool growTo(Array a, size_t newCapacity) {
  a->capacity = newCapacity;
  void* aux = realloc(a->array, newCapacity * a->elementSize);
  if (aux == NULL) return false;
  a->array = aux;
  return true;
}

bool growBy(Array a, size_t extraCapacity) {
  return growTo(a, a->capacity + extraCapacity);
}
