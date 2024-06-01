#include <CircularBuffer/circularBuffer.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct CircularBufferADT {
  size_t elementSize;
  size_t capacity;
  size_t readIdx;
  size_t writeIdx;
  size_t toRead;
  // Flexible array member. In ArrayADT it wasn't a good idea to use this
  // because the array could grow but here it'll be of fixed size after
  // initialization.
  uint8_t buffer[];
} CircularBufferADT;

void copynEleAt(CircularBuffer cb, size_t idx, const void* eleArr, size_t n);
void copyEleAt(CircularBuffer cb, size_t idx, const void* ele);
void increaseWriteIdx(CircularBuffer cb);
void increaseReadIdx(CircularBuffer cb);
void decreaseWriteIdx(CircularBuffer cb);

CircularBuffer CB_initialize(size_t elementSize, size_t capacity) {
  if (elementSize == 0) {
    errno = EINVAL;
    return NULL;
  }
  CircularBuffer cb = malloc(sizeof(CircularBufferADT) + elementSize * capacity);
  if (cb == NULL) return NULL;
  cb->elementSize = elementSize;
  cb->capacity = capacity;
  cb->readIdx = 0;
  cb->writeIdx = 0;
  cb->toRead = 0;

  return cb;
}

void CB_writeNext(CircularBuffer cb, void* ele) {
  copyEleAt(cb, cb->writeIdx, ele);
  increaseWriteIdx(cb);
}

void* CB_readNext(CircularBuffer cb) {
  if (cb->toRead == 0) return NULL;
  void* ele = cb->buffer + cb->readIdx * cb->elementSize;
  increaseReadIdx(cb);
  return ele;
}

void CB_removePrev(CircularBuffer cb) {
  decreaseWriteIdx(cb);
}

size_t CB_getLen(CircularBuffer cb) {
  return cb->toRead;
}

void CB_free(CircularBuffer cb) {
  free(cb);
}

void copynEleAt(CircularBuffer cb, size_t idx, const void* eleArr, size_t n) {
  memcpy(cb->buffer + cb->elementSize * idx, eleArr, cb->elementSize * n);
}

void copyEleAt(CircularBuffer cb, size_t idx, const void* ele) {
  copynEleAt(cb, idx, ele, 1);
}

void increaseWriteIdx(CircularBuffer cb) {
  if (cb->writeIdx == cb->readIdx && cb->toRead > 0) increaseReadIdx(cb);
  cb->writeIdx = (cb->writeIdx + 1) % cb->capacity;
  cb->toRead = cb->toRead < cb->capacity ? cb->toRead + 1 : cb->capacity;
}

void increaseReadIdx(CircularBuffer cb) {
  if (cb->readIdx == cb->writeIdx && cb->toRead == 0) return;
  cb->readIdx = (cb->readIdx + 1) % cb->capacity;
  cb->toRead = cb->toRead > 0 ? cb->toRead - 1 : 0;
}

void decreaseWriteIdx(CircularBuffer cb) {
  if (cb->toRead == 0) return;
  if (cb->writeIdx == 0) cb->writeIdx = cb->capacity - 1;
  else --cb->writeIdx;
  cb->writeIdx = (cb->writeIdx + 1) % cb->capacity;
  cb->toRead = cb->toRead > cb->capacity ? cb->toRead - 1 : 0;
}
