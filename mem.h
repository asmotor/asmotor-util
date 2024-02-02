/*  Copyright 2008-2022 Carsten Elton Sorensen

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

#ifndef UTIL_MEM_H_INCLUDED_
#define UTIL_MEM_H_INCLUDED_

#include <stdlib.h>

#include "util.h"

#if defined(ASMOTOR_FAKE_ALLOC)
extern void*
mem_AllocImpl(size_t size, const char* filename, int lineNumber);

INLINE void*
mem_ReallocImpl(void* memory, size_t size, const char* filename, int lineNumber) {
	void* d = mem_AllocImpl(size, filename, lineNumber);
	if (memory != NULL)
		memcpy(d, memory, size);
	return d;
}

INLINE void*
mem_Alloc(size_t size) {
	return mem_AllocImpl(size, NULL, 0);
}

INLINE void*
mem_Realloc(void* memory, size_t size) {
	return mem_ReallocImpl(memory, size, NULL, 0);
}

INLINE void
mem_Free(void* memory) {
}

INLINE void
mem_ShowLeaks(void) {
}
#elif defined(ASMOTOR_INLINE_MEMORY)
INLINE void*
mem_AllocImpl(size_t size, const char* filename, int lineNumber) {
	return malloc(size);
}

INLINE void*
mem_ReallocImpl(void* memory, size_t size, const char* filename, int lineNumber) {
	return realloc(memory, size);
}

INLINE void*
mem_Alloc(size_t size) {
	return malloc(size);
}

INLINE void*
mem_Realloc(void* memory, size_t size) {
	return realloc(memory, size);
}

INLINE void
mem_Free(void* memory) {
	free(memory);
}

INLINE void
mem_ShowLeaks(void) {
}
#elif defined(_DEBUG)
extern void*
mem_AllocImpl(size_t size, const char* filename, int lineNumber);

extern void*
mem_ReallocImpl(void* memory, size_t size, const char* filename, int lineNumber);

#define mem_Alloc(size) mem_AllocImpl(size, __FILE__, __LINE__)
#define mem_Realloc(memory, size) mem_ReallocImpl(memory, size, __FILE__, __LINE__)
#else
extern void*
mem_Alloc(size_t size);

extern void*
mem_Realloc(void* memory, size_t size);
#endif

extern void
mem_Free(void* memory);

extern void
mem_ShowLeaks(void);

#endif
