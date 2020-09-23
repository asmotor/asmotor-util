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

#define IN_VEC_C_

#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include "set.h"

typedef struct {
    free_t free;
    intptr_t userData;
    uint32_t allocatedElements;
    uint32_t totalElements;
    intptr_t* elements;
} vec_t;

#include "vec.h"


extern vec_t* 
vec_CreateLength(free_t free, size_t size) {
    vec_t* vec = (vec_t*) mem_Alloc(sizeof(vec_t));
    vec->free = free;
    vec->userData = 0;
    vec->allocatedElements = size;
    vec->totalElements = 0;
    vec->elements = mem_Alloc(sizeof(intptr_t) * vec->allocatedElements);

    return vec;
}


extern void
vec_PushBack(vec_t* vec, intptr_t element) {
    if (vec->totalElements >= vec->allocatedElements) {
        vec->allocatedElements += (vec->allocatedElements >> 1) + 1;
        vec->elements = mem_Realloc(vec->elements, sizeof(intptr_t) * vec->allocatedElements);
    }
    vec->elements[vec->totalElements++] = element;
}


extern size_t
vec_Count(vec_t* vec) {
    assert(vec != NULL);
    return vec->totalElements;
}


extern void
vec_Free(vec_t* vec) {
    assert(vec != NULL);

    for (uint32_t i = 0; i < vec->totalElements; ++i) {
        vec->free(vec->userData, vec->elements[i]);
    }
    mem_Free(vec->elements);
    mem_Free(vec);
}


extern void
vec_RemoveAt(vec_t* vec, ssize_t index) {
	assert(index >= 0 && index < vec->totalElements);

	vec->free(vec->userData, vec->elements[index]);
	vec->totalElements -= 1;
	if (index < vec->totalElements) {
		memcpy(&vec[index], &vec[index + 1], vec->totalElements - index);
	}
}


extern intptr_t
vec_ElementAt(vec_t* vec, ssize_t index) {
	assert(index >= 0 && index < vec->totalElements);
	return vec->elements[index];
}


extern void
vec_SetAt(vec_t* vec, intptr_t element, ssize_t index) {
	assert(index >= 0 && index < vec->totalElements);
	vec->free(vec->userData, vec->elements[index]);
	vec->elements[index] = element;
}
