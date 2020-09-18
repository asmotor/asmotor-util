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

#if !defined(UTIL_VEC_H_INCLUDED_)
#define UTIL_VEC_H_INCLUDED_

#include <stdbool.h>
#include <stdint.h>

#include "mem.h"
#include "str.h"
#include "set.h"

#if !defined(IN_VEC_C_)
typedef void vec_t;
#endif

extern vec_t* 
vec_Create(free_t free);

extern void
vec_PushBack(vec_t* vec, intptr_t element);

extern void
vec_Free(vec_t* vec);

extern size_t
vec_Count(vec_t* vec);

#endif
