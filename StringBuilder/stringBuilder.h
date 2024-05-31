#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <stdio.h>
#include "arrayADT.h"

typedef Array StringBuilder;

StringBuilder SB_initialize(size_t initialCapacity);
bool SB_push(StringBuilder sb, char c);
size_t SB_len(StringBuilder sb);
char* SB_toString(StringBuilder sb);
void SB_print(StringBuilder sb);
void SB_free(StringBuilder sb);

#endif
