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

#include <stdbool.h>
#include <string.h>

#include "map.h"
#include "protos.h"
#include "str.h"
#include "strpmap.h"
#include "util.h"

static bool
equals(intptr_t userData, intptr_t element1, intptr_t element2) {
	const char* s1 = (const char*) element1;
	const char* s2 = (const char*) element2;
	return s1 == s2 || strcmp(s1, s2) == 0;
}

static bool
equalsi(intptr_t userData, intptr_t element1, intptr_t element2) {
	const char* s1 = (const char*) element1;
	const char* s2 = (const char*) element2;
	return s1 == s2 || _stricmp(s1, s2) == 0;
}

static uint32_t
hash(intptr_t userData, intptr_t element) {
	const char* str = (const char*) element;
	return str_JenkinsHashLength(str, strlen(str));
}

static uint32_t
hashi(intptr_t userData, intptr_t element) {
	const char* str = (const char*) element;
	return str_JenkinsHashLength(str, strlen(str));
}

static void
nopFree(intptr_t userData, intptr_t element) {
}

extern strpmap_t*
strpmap_Create(free_t valueFree) {
	return (strpmap_t*) map_Create(equals, hash, nopFree, valueFree);
}

extern strpmap_t*
strpmap_CreateI(free_t valueFree) {
	return (strpmap_t*) map_Create(equalsi, hashi, nopFree, valueFree);
}

typedef struct {
	strpmap_foreach_t forEach;
	intptr_t data;
} foreach_data_t;

static void 
map_foreach(map_t* map, intptr_t key, intptr_t value, intptr_t data) {
	foreach_data_t* d = (foreach_data_t*) data;
	d->forEach((strpmap_t*) map, (const char*) key, value, d->data);
}

extern void
strpmap_ForEachKeyValue(strpmap_t* map, strpmap_foreach_t forEach, intptr_t data) {
	foreach_data_t fd = { forEach, data };
	map_ForEachKeyValue((map_t*) map, map_foreach, (intptr_t) &fd);
}



