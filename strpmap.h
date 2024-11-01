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

#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "map.h"
#include "protos.h"
#include "types.h"
#include "util.h"

#ifndef IN_STRPMAP_C_
struct StringPointerMap;
typedef struct StringPointerMap strpmap_t;
#endif

typedef void (*strpmap_foreach_t)(strpmap_t* map, const char* key, intptr_t value, intptr_t data);


extern strpmap_t*
strpmap_Create(free_t valueFree);

extern strpmap_t*
strpmap_CreateI(free_t valueFree);

INLINE strpmap_t*
strpmap_CreateSubMap(strpmap_t* map) {
	return (strpmap_t*) map_CreateSubMap((map_t*) map);
}

INLINE void
strpmap_Clear(strpmap_t* map) {
	map_Clear((map_t*) map);
}

INLINE void
strpmap_Free(strpmap_t* map) {
	map_Free((map_t*) map);
}

INLINE void
strpmap_Insert(strpmap_t* map, const char* key, intptr_t value) {
	map_Insert((map_t*) map, (intptr_t) key, value);
}

INLINE void
strpmap_Remove(strpmap_t* map, const char* key) {
	map_Remove((map_t*) map, (intptr_t) key);
}

INLINE bool
strpmap_Value(strpmap_t* map, const char* key, intptr_t* value) {
	return map_Value((map_t*) map, (intptr_t) key, value);
}

INLINE bool
strpmap_HasKey(strpmap_t* map, const char* key) {
	return map_HasKey((map_t*) map, (intptr_t) key);
}

INLINE ssize_t
strpmap_Count(strpmap_t* map) {
	return map_Count((map_t*) map);
}

extern void
strpmap_ForEachKeyValue(strpmap_t* map, strpmap_foreach_t forEach, intptr_t data);
