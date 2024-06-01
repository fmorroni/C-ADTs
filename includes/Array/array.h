#ifndef ARRAY_ADT_H
#define ARRAY_ADT_H

#include <stdbool.h>
#include <stdio.h>

typedef struct ArrayCDT* Array;

typedef void (*FreeEleFn)(void* ele);
typedef void (*PrintFn)(void* ele);
typedef void (*MapFn)(void* mappedEle, void* ele, size_t idx);

void* Array_initialize(size_t elementSize, size_t initialCapacity, PrintFn printFn, FreeEleFn freeFn);
void Array_free(Array a);
bool Array_push(Array a, const void* ele);
size_t Array_getLen(Array a);
void* Array_getEle(Array a, size_t idx);
bool Array_setEle(Array a, size_t idx, void* ele);
void Array_print(Array a);
void Array_printInfo(Array a);
Array Array_map(Array a, MapFn mapFn, size_t newElemSize, PrintFn newTypePrintFn, FreeEleFn freeFn);
Array Array_fromVanillaArray(const void* array, size_t length, size_t elementSize, PrintFn printFn, FreeEleFn freeFn);
bool Array_concat(Array dst, Array src);
const void* Array_getVanillaArray(Array a);
void* Array_copyVanillaArrayInto(Array a, void* array);
void* Array_getVanillaArrayCopy(Array a);

#endif
