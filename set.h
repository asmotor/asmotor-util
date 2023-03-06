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

#if !defined(UTIL_SET_H_INCLUDED_)
#define UTIL_SET_H_INCLUDED_

#include <stdbool.h>
#include <stdint.h>

#include "mem.h"
#include "protos.h"
#include "str.h"

#if !defined(IN_SET_C_)
struct Set;
typedef struct Set set_t;
#endif

typedef bool (*predicate_t)(intptr_t userData, intptr_t predicateData, intptr_t element);

extern set_t* 
set_Create(equals_t equals, hash_t hash, free_t free);

extern void
set_Clear(set_t* set);

extern bool
set_Exists(set_t* set, intptr_t element);

extern bool
set_Find(set_t* set, predicate_t predicate, intptr_t predicateData, intptr_t* value);

extern bool
set_Value(set_t* set, intptr_t element, intptr_t* value);

extern void
set_Insert(set_t* set, intptr_t element);

extern void
set_Remove(set_t* set, intptr_t element);

extern void
set_Free(set_t* set);

extern ssize_t
set_Count(set_t* set);

extern void
set_ForEachElement(set_t* set, void (*forEach)(intptr_t element, intptr_t data), intptr_t data);

extern intptr_t*
set_ToArray(set_t* set, copy_t copy, ssize_t* totalElements);

extern void
set_SetUserData(set_t* set, intptr_t data);

extern intptr_t
set_GetUserData(set_t* set);

extern set_t*
set_CreateSubSet(set_t* set);

#endif
