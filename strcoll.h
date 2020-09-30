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

#if !defined(UTIL_STRCOLL_H_INCLUDED_)
#define UTIL_STRCOLL_H_INCLUDED_

#include "set.h"
#include "vec.h"
#include "map.h"

// String set functions

extern set_t*
strset_Create();

INLINE bool
strset_Exists(set_t* set, const string* element) {
    return set_Exists(set, (intptr_t) element);
}

INLINE void
strset_Insert(set_t* set, string* element) {
    set_Insert(set, (intptr_t) str_Copy(element));
}

INLINE void
strset_Remove(set_t* set, const string* element) {
    set_Remove(set, (intptr_t) element);
}


// String vector functions

extern vec_t*
#if defined(_DEBUG)
strvec_CreateDebug(const char* filename, int lineNumber);
#define strvec_Create() strvec_CreateDebug(__FILE__, __LINE__)
#else
strvec_Create(void);
#endif

extern vec_t*
strvec_Copy(vec_t* collection);

INLINE void
strvec_PushBack(vec_t* vec, string* element) {
	vec_PushBack(vec, (intptr_t) str_Copy(element));
}

INLINE string*
strvec_StringAt(vec_t* vec, size_t index) {
	return str_Copy((string *) vec_ElementAt(vec, index));
}

INLINE void
strvec_SetAt(vec_t* vec, size_t index, string* element) {
	vec_SetAt(vec, index, (intptr_t) str_Copy(element));
}

#define strvec_Freeze vec_Freeze
#define strvec_Frozen vec_Frozen
#define strvec_Free vec_Free
#define strvec_Count vec_Count
#define strvec_RemoveAt vec_RemoveAt


// String map functions

extern map_t*
#if defined(_DEBUG)
strmap_CreateDebug(free_t valueFree, const char* filename, int lineNumber);
#define strmap_Create(valueFree) strmap_CreateDebug(valueFree, __FILE__, __LINE__)
#else
strmap_Create(free_t valueFree);
#endif

INLINE bool
strmap_Value(map_t* map, const string* key, intptr_t* value) {
    return map_Value(map, (intptr_t) key, value);
}

INLINE void
strmap_Insert(map_t* map, const string* key, intptr_t value) {
    map_Insert(map, (intptr_t) str_Copy(key), value);
}

INLINE bool
strmap_HasKey(map_t* map, const string* key) {
    return map_HasKey(map, (intptr_t) key);
}

#define strmap_Free map_Free

#endif
