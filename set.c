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

#define IN_SET_C_

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "str.h"

#define SET_HASH_SIZE 32

struct ListEntry {
	uint32_t allocatedElements;
	uint32_t totalElements;
	intptr_t* elements;
};

typedef struct ListEntry SListEntry;

typedef bool (*equals_t)(intptr_t userData, intptr_t element1, intptr_t element2);
typedef uint32_t (*hash_t)(intptr_t userData, intptr_t element);
typedef void (*free_t)(intptr_t userData, intptr_t element);

typedef struct Set {
	equals_t equals;
	hash_t hash;
	free_t free;
	intptr_t userData;
	SListEntry lists[SET_HASH_SIZE];
} set_t;

#include "set.h"

extern set_t* 
set_Create(equals_t equals, hash_t hash, free_t free) {
	set_t* set = (set_t*) mem_Alloc(sizeof(set_t));
	set->equals = equals;
	set->hash = hash;
	set->free = free;

	for (uint32_t i = 0; i < SET_HASH_SIZE; ++i) {
		set->lists[i].allocatedElements = 0;
		set->lists[i].totalElements = 0;
		set->lists[i].elements = NULL;
	}

	return set;
}

static bool
privateExists(set_t* set, SListEntry* list, intptr_t element) {
	assert(set != NULL && list != NULL);

	if (list->totalElements > 0 && list->elements != NULL) {
		for (uint32_t i = 0; i < list->totalElements; ++i) {
			if (set->equals(set->userData, list->elements[i], element))
				return true;
		}
	}
	return false;
}

static SListEntry*
privateListForElement(set_t* set, intptr_t element) {
	assert(set != NULL);
	return &set->lists[set->hash(set->userData, element) % SET_HASH_SIZE];
}

extern bool
set_Exists(set_t* set, intptr_t element) {
	assert(set != NULL);
	SListEntry* list = privateListForElement(set, element);
	return privateExists(set, list, element);
}

extern bool
set_Find(set_t* set, predicate_t predicate, intptr_t predicateData, intptr_t* value) {
	assert(set != NULL && predicate != NULL && value != NULL);
	for (uint32_t i = 0; i < SET_HASH_SIZE; ++i) {
		SListEntry* list = &set->lists[i];
		for (uint32_t j = 0; j < list->totalElements; ++j) {
			if (predicate(set->userData, predicateData, list->elements[j])) {
				*value = list->elements[j];
				return true;
			}
		}
	}
	return false;
}

extern void
set_Insert(set_t* set, intptr_t element) {
	assert(set != NULL);

	SListEntry* list = privateListForElement(set, element);

	if (!privateExists(set, list, element)) {
		if (list->elements == NULL || list->allocatedElements == list->totalElements) {
			list->allocatedElements = list->allocatedElements * 2 + 4;
			list->elements = mem_Realloc(list->elements, list->allocatedElements * sizeof(void *));
		}
		list->elements[list->totalElements++] = element;
	}
}

extern void
set_Remove(set_t* set, intptr_t element) {
	assert(set != NULL);

	SListEntry* list = privateListForElement(set, element);

	if (list->totalElements > 0 && list->elements != NULL) {
		for (uint32_t i = 0; i < list->totalElements; ++i) {
			if (set->equals(set->userData, list->elements[i], element)) {
				set->free(set->userData, list->elements[i]);
				list->elements[i] = list->elements[--(list->totalElements)];
				return;
			}
		}
	}
}

extern void
set_ForEachElement(set_t* set, void (*forEach)(intptr_t element, intptr_t data), intptr_t data) {
	assert(set != NULL && forEach != NULL);

	for (uint32_t i = 0; i < SET_HASH_SIZE; ++i) {
		SListEntry* list = &set->lists[i];
		if (list->elements != NULL) {
			for (uint32_t j = 0; j < list->totalElements; ++j) {
				forEach(list->elements[j], data);
			}
		}
	}
}

static void
increment(intptr_t element, intptr_t data) {
	*(size_t*)data += 1;
}

extern ssize_t
set_Count(set_t* set) {
	assert(set != NULL);

	size_t count = 0;
	set_ForEachElement(set, increment, (intptr_t) &count);

	return count;
}

extern void
set_Free(set_t* set) {
	assert(set != NULL);

	for (uint32_t i = 0; i < SET_HASH_SIZE; ++i) {
		SListEntry* list = &set->lists[i];
		if (list->elements != NULL) {
			for (uint32_t j = 0; j < list->totalElements; ++j) {
				set->free(set->userData, list->elements[j]);
			}
			mem_Free(list->elements);
		}
	}
	mem_Free(set);
}

extern intptr_t*
set_ToArray(set_t* set, copy_t copy, ssize_t* totalElements) {
	assert(set != NULL && copy != NULL && totalElements != NULL);

	*totalElements = set_Count(set);
	intptr_t* array = mem_Alloc(*totalElements * sizeof(intptr_t));

	size_t arrayIndex = 0;
	for (uint32_t i = 0; i < SET_HASH_SIZE; ++i) {
		SListEntry* list = &set->lists[i];
		if (list->elements != NULL) {
			for (uint32_t j = 0; j < list->totalElements; ++j) {
				array[arrayIndex++] = copy(set->userData, list->elements[j]);
			}
		}
	}
	
	return array;
}

extern void
set_SetUserData(set_t* set, intptr_t data) {
	assert(set != NULL);
	set->userData = data;
}

extern intptr_t
set_GetUserData(set_t* set) {
	assert(set != NULL);
	return set->userData;
}


