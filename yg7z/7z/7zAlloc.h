/* 7zAlloc.h -- Allocation functions
2010-10-29 : Igor Pavlov : Public domain */

#ifndef __7Z_ALLOC_H
#define __7Z_ALLOC_H

#include <stdlib.h>

#include "Types.h"

EXTERN_C_BEGIN

void *SzAlloc(void *p, size_t size);
void SzFree(void *p, void *address);

void *SzAllocTemp(void *p, size_t size);
void SzFreeTemp(void *p, void *address);

EXTERN_C_END

#endif
