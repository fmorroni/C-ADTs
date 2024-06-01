#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stddef.h>

typedef struct CircularBufferADT* CircularBuffer;

CircularBuffer CB_initialize(size_t elementSize, size_t capacity);
void CB_writeNext(CircularBuffer cb, void* ele);
void* CB_readNext(CircularBuffer cb);
size_t CB_getLen(CircularBuffer cb);
void CB_free(CircularBuffer cb);

#endif
