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

#if !defined(UTIL_MAP_H_INCLUDED_)
#define UTIL_MAP_H_INCLUDED_

#include <stdbool.h>
#include <stdio.h>

#include "protos.h"

#ifndef IN_MAP_C_
struct Map;
typedef struct Map map_t;
#endif

typedef void (*map_foreach_t)(map_t* map, intptr_t key, intptr_t value, intptr_t data);
typedef bool (*map_predicate_t)(map_t* map, intptr_t predicateData, intptr_t key, intptr_t value);


extern map_t*
#if defined(_DEBUG)
map_CreateDebug(equals_t keyEquals, hash_t keyHash, free_t keyFree, free_t valueFree, const char* filename, int lineNumber);
#define map_Create(keyEquals, keyHash, keyFree, valueFree) map_CreateDebug(keyEquals, keyHash, keyFree, valueFree, __FILE__, __LINE__)
#else
map_Create(equals_t keyEquals, hash_t keyHash, free_t keyFree, free_t valueFree);
#endif

extern map_t*
map_CreateSubMap(map_t* map);

extern void
map_Clear(map_t* map);

extern void
map_Free(map_t* map);

extern void
map_Insert(map_t* map, intptr_t key, intptr_t value);

extern void
map_Remove(map_t* set, intptr_t key);

extern bool
map_Value(map_t* map, intptr_t key, intptr_t* value);

extern bool
map_HasKey(map_t* map, intptr_t key);

extern void
map_ForEachKeyValue(map_t* map, map_foreach_t forEach, intptr_t data);

extern ssize_t
map_Count(map_t* map);

extern bool
map_Find(map_t* map, map_predicate_t predicate, intptr_t predicateData, intptr_t* key, intptr_t* value);


#endif
