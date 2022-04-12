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

#if !defined(UTIL_VEC_H_INCLUDED_)
#define UTIL_VEC_H_INCLUDED_

#include <stdint.h>

#include "mem.h"
#include "str.h"
#include "set.h"

#if !defined(IN_VEC_C_)
typedef void vec_t;
#endif

extern vec_t* 
#if defined(_DEBUG)
vec_CreateLengthDebug(free_t free, size_t size, const char* filename, int lineNumber);
#define vec_CreateLength(free, size) vec_CreateLengthDebug(free, size, __FILE__, __LINE__)
#else
vec_CreateLength(free_t free, size_t size);
#endif

INLINE vec_t* 
#if defined(_DEBUG)
vec_CreateDebug(free_t free, const char* filename, int lineNumber) {
	return vec_CreateLengthDebug(free, 16, filename, lineNumber);
#define vec_Create(free) vec_CreateDebug(free, __FILE__, __LINE__)
#else
vec_Create(free_t free) {
	return vec_CreateLength(free, 16);
#endif
}

extern void
vec_PushBack(vec_t* vec, intptr_t element);

extern void
vec_Free(vec_t* vec);

extern size_t
vec_Count(vec_t* vec);

extern void
vec_RemoveAt(vec_t* vec, ssize_t index);

extern intptr_t
vec_ElementAt(vec_t* vec, ssize_t index);

extern void
vec_SetAt(vec_t* vec, ssize_t index, intptr_t element);

extern vec_t*
vec_Freeze(vec_t* vec);

extern bool
vec_Frozen(vec_t* vec);

extern vec_t*
vec_Copy(vec_t* vec, copy_t copy);


#endif
