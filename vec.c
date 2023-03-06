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

#define IN_VEC_C_

#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include "set.h"

typedef struct Vector {
	uint32_t refCount;
	free_t free;
	intptr_t userData;
	uint32_t allocatedElements;
	uint32_t totalElements;
	intptr_t* elements;
} vec_t;

#include "vec.h"


static void
growVector(vec_t* vec) {
	assert (!vec_Frozen(vec));
	vec->allocatedElements += (vec->allocatedElements >> 1) + 1;
	vec->elements = mem_Realloc(vec->elements, sizeof(intptr_t) * vec->allocatedElements);
}


extern vec_t* 
#if defined(_DEBUG)
vec_CreateLengthDebug(free_t free, size_t size, const char* filename, int lineNumber) {
	vec_t* vec = (vec_t*) mem_AllocImpl(sizeof(vec_t), filename, lineNumber);
#else
vec_CreateLength(free_t free, size_t size) {
	vec_t* vec = (vec_t*) mem_Alloc(sizeof(vec_t));
#endif
	vec->refCount = 0;
	vec->free = free;
	vec->userData = 0;
	vec->allocatedElements = size == 0 ? 1 : (uint32_t) size;
	vec->totalElements = 0;
#if defined(_DEBUG)
	vec->elements = mem_AllocImpl(sizeof(intptr_t) * vec->allocatedElements, filename, lineNumber);
#else
	vec->elements = mem_Alloc(sizeof(intptr_t) * vec->allocatedElements);
#endif

	return vec;
}


extern void
vec_PushBack(vec_t* vec, intptr_t element) {
	assert (!vec_Frozen(vec));
	if (vec->totalElements >= vec->allocatedElements)
		growVector(vec);
	vec->elements[vec->totalElements++] = element;
}


extern size_t
vec_Count(vec_t* vec) {
	assert (vec != NULL);
	return vec->totalElements;
}


extern void
vec_Clear(vec_t* vec) {
	assert(vec != NULL);
	assert(!vec_Frozen(vec));

	for (uint32_t i = 0; i < vec->totalElements; ++i) {
		vec->free(vec->userData, vec->elements[i]);
	}
	vec->totalElements = 0;
}


extern void
vec_Free(vec_t* vec) {
	assert(vec != NULL);

	if (vec_Frozen(vec)) {
		vec->refCount -= 1;
	}

	if (!vec_Frozen(vec)) {
		for (uint32_t i = 0; i < vec->totalElements; ++i) {
			vec->free(vec->userData, vec->elements[i]);
		}
		mem_Free(vec->elements);
		mem_Free(vec);
	}
}


extern void
vec_RemoveAt(vec_t* vec, ssize_t index) {
	assert (vec != NULL);
	assert (index >= 0 && index < vec->totalElements);
	assert (!vec_Frozen(vec));

	vec->free(vec->userData, vec->elements[index]);
	vec->totalElements -= 1;
	if (index < (ssize_t) vec->totalElements) {
		memmove(&vec->elements[index], &vec->elements[index + 1], sizeof(intptr_t) * (vec->totalElements - index));
	}
}


extern intptr_t
vec_ElementAt(vec_t* vec, ssize_t index) {
	assert (vec != NULL);
	assert (index >= 0 && index < vec->totalElements);
	return vec->elements[index];
}


extern intptr_t
vec_SetAt(vec_t* vec, ssize_t index, intptr_t element) {
	assert (vec != NULL);
	assert (!vec_Frozen(vec));
	assert(index >= 0 && index < vec->totalElements);

	intptr_t r = vec->elements[index];
	if (element != vec->elements[index]) {
		vec->free(vec->userData, vec->elements[index]);
		vec->elements[index] = element;
	}
	return r;
}


extern void
vec_InsertAt(vec_t* vec, ssize_t index, intptr_t element) {
	assert (vec != NULL);
	assert (index >= 0 && index <= vec->totalElements);
	assert (!vec_Frozen(vec));

	if (vec->totalElements == vec->allocatedElements)
		growVector(vec);

	if (index < (ssize_t) vec->totalElements)
		memmove(&vec->elements[index + 1], &vec->elements[index], sizeof(intptr_t) * (vec->totalElements - index));

	vec->elements[index] = element;
	vec->totalElements += 1;
}


extern vec_t*
vec_Freeze(vec_t* vec) {
	assert (vec != NULL);
	assert (!vec_Frozen(vec));
	vec->refCount = 1;
	return vec;
}


extern bool
vec_Frozen(vec_t* vec) {
	assert (vec != NULL);
	return vec->refCount != 0;
}


extern vec_t*
vec_Copy(vec_t* vec, copy_t copy) {
	if (vec != NULL) {
		if (vec_Frozen(vec)) {
			vec->refCount += 1;
			return vec;
		}

		vec_t* dest = vec_CreateLength(vec->free, vec->totalElements);
		for (size_t i = 0; i < vec_Count(vec); ++i) {
			vec_PushBack(dest, copy(vec->userData, vec_ElementAt(vec, i)));
		}

		return dest;
	} else {
		return NULL;
	}
}
