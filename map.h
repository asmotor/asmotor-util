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

#if !defined(UTIL_MAP_H_INCLUDED_)
#define UTIL_MAP_H_INCLUDED_

#include "set.h"


#if !defined(IN_MAP_C_)
typedef void map_t;
#endif

typedef void (*map_foreach_t)(intptr_t key, intptr_t value, intptr_t data);


extern map_t*
map_Create(equals_t keyEquals, hash_t keyHash, free_t keyFree, free_t valueFree);

extern void
map_Insert(map_t* map, intptr_t key, intptr_t value);

extern bool
map_Value(map_t* map, intptr_t key, intptr_t* value);

extern bool
map_HasKey(map_t* map, intptr_t key);

extern void
map_ForEachKeyValue(map_t* map, map_foreach_t forEach, intptr_t data);

extern ssize_t
map_Count(map_t* map);


#endif
