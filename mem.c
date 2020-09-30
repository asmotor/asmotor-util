/*  Copyright 2008-2017 Carsten Elton Sorensen

    This file is part of ASMotor.

    ASMotor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ASMotor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ASMotor.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "util.h"
#include "mem.h"
#include "lists.h"

#define HEADERSIZE sizeof(SMemoryChunk)

typedef struct MemoryChunk {
    list_Data(struct MemoryChunk);
    size_t size;
#if defined(_DEBUG)
    const char* filename;
    int lineNumber;
#endif
} SMemoryChunk;

static SMemoryChunk* g_memoryList;

static void*
#if defined(_DEBUG)
CheckMemPointer(SMemoryChunk* chunk, size_t size, const char* filename, int lineNumber) {
#else
checkMemPointer(SMemoryChunk* chunk, size_t size) {
#endif
    if (!chunk) {
        fprintf(stderr, "Unable to allocate memory. Critical error, exiting.\n");
        exit(EXIT_FAILURE);
    }

    chunk->size = size;
#if defined(_DEBUG)
    chunk->filename = filename;
    chunk->lineNumber = lineNumber;
#endif
    list_Insert(g_memoryList, chunk);

    return (char*) chunk + HEADERSIZE;
}

#if defined(_DEBUG)
void*
mem_AllocImpl(size_t size, const char *filename, int lineNumber) {
    assert (size != 0);
    uint8_t* mem = CheckMemPointer(malloc(size + HEADERSIZE), size, filename, lineNumber);
    for (size_t i = 0; i < size; ++i) {
        switch (i & 3) {
            case 0: mem[i] = 0xDE; break;
            case 1: mem[i] = 0xAD; break;
            case 2: mem[i] = 0xBE; break;
            case 3: mem[i] = 0xEF; break;
        }
    }
    return mem;
}
#else

void*
mem_Alloc(size_t size) {
    assert (size != 0);
    return checkMemPointer(malloc(size + HEADERSIZE), size);
}

#endif

void*
#if defined(_DEBUG)
mem_ReallocImpl(void* memory, size_t size, const char* filename, int lineNumber) {
#else
mem_Realloc(void* memory, size_t size) {
#endif
    if (memory == NULL) {
#if defined(_DEBUG)
        return mem_AllocImpl(size, filename, lineNumber);
#else
        return mem_Alloc(size);
#endif
    } else if (size == 0) {
        mem_Free(memory);
        return NULL;
    } else {
        SMemoryChunk* chunk = (SMemoryChunk*) ((char*) memory - HEADERSIZE);
        list_Remove(g_memoryList, chunk);

#if defined(_DEBUG)
        return CheckMemPointer(realloc(chunk, size + HEADERSIZE), size, filename, lineNumber);
#else
        return checkMemPointer(realloc(chunk, size + HEADERSIZE), size);
#endif
    }
}

void
mem_Free(void* memory) {
    if (memory != NULL) {
        SMemoryChunk* chunk = (SMemoryChunk*) ((char*) memory - HEADERSIZE);

        assert (chunk->size != 0);

        list_Remove(g_memoryList, chunk);
        chunk->size = 0;

        free(chunk);
    }
}


void
mem_ShowLeaks(void) {
#if defined(_DEBUG)
    for (SMemoryChunk* chunk = g_memoryList; chunk != NULL; chunk = list_GetNext(chunk)) {
        printf("Leak at %s:%d\n", chunk->filename, chunk->lineNumber);
    }
#endif
}
