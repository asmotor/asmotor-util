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

#include "strcoll.h"

static bool
stringEquals(intptr_t userData, intptr_t element1, intptr_t element2) {
    return str_Equal((string*) element1, (string*) element2);
}

static uint32_t
stringHash(intptr_t userData, intptr_t element) {
    return str_JenkinsHash((string*) element);
}

static void
stringFree(intptr_t userData, intptr_t element) {
    str_Free((string*) element);
}

static intptr_t
stringCopy(intptr_t userData, intptr_t element) {
    return (intptr_t) str_Copy((string*) element);
}


// String set functions

extern set_t*
strset_Create() {
    return set_Create(stringEquals, stringHash, stringFree);
}


// String map functions

extern map_t*
#if defined(_DEBUG)
strmap_CreateDebug(free_t valueFree, const char* filename, int lineNumber) {
    return map_CreateDebug(stringEquals, stringHash, stringFree, valueFree, filename, lineNumber);
#else
strmap_Create(free_t valueFree) {
    return map_Create(stringEquals, stringHash, stringFree, valueFree);
#endif
}


// String vector functions

extern vec_t* 
#if defined(_DEBUG)
strvec_CreateDebug(const char* filename, int lineNumber) {
	return vec_CreateDebug(stringFree, filename, lineNumber);
#else
strvec_Create(void) {
	return vec_Create(stringFree);
#endif
}


extern vec_t* 
strvec_CreateLength(size_t size) {
	return vec_CreateLength(stringFree, size);
}


extern vec_t*
strvec_Copy(vec_t* vec) {
	return vec_Copy(vec, stringCopy);
}
