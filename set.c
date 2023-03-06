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

#include "protos.h"
#include "str.h"
#include "vec.h"

#define SET_HASH_SIZE 29

struct ListEntry {
	uint32_t allocatedElements;
	uint32_t totalElements;
	intptr_t* elements;
};

typedef struct ListEntry SListEntry;

typedef struct Set {
	equals_t equals;
	hash_t hash;
	free_t free;
	intptr_t userData;
	SListEntry lists[SET_HASH_SIZE];
	vec_t* subSets;
} set_t;

#include "set.h"


static void
vec_set_free(intptr_t userData, intptr_t element) {
	set_Free((set_t*) element);
}


extern set_t* 
set_Create(equals_t equals, hash_t hash, free_t free) {
	set_t* set = (set_t*) mem_Alloc(sizeof(set_t));
	set->equals = equals;
	set->hash = hash;
	set->free = free;
	set->subSets = vec_Create(vec_set_free);

	for (uint32_t i = 0; i < SET_HASH_SIZE; ++i) {
		set->lists[i].allocatedElements = 0;
		set->lists[i].totalElements = 0;
		set->lists[i].elements = NULL;
	}

	return set;
}


static uint32_t
hashElement(set_t* set, intptr_t element) {
	return set->hash(set->userData, element) % SET_HASH_SIZE;
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

	for (uint32_t i; i < vec_Count(set->subSets); ++i) {
		if (set_Find((set_t *) vec_ElementAt(set->subSets, i), predicate, predicateData, value))
			return true;
	}

	return false;
}

static bool
internal_Value(set_t* set, uint32_t hash, intptr_t element, intptr_t* value) {
	assert(set != NULL);

	SListEntry* list = &set->lists[hash];

	if (list->totalElements > 0 && list->elements != NULL) {
		for (uint32_t i = 0; i < list->totalElements; ++i) {
			if (set->equals(set->userData, list->elements[i], element)) {
				*value = list->elements[i];
				return true;
			}
		}
	}

	for (uint32_t i; i < vec_Count(set->subSets); ++i) {
		if (internal_Value((set_t *) vec_ElementAt(set->subSets, i), hash, element, value))
			return true;
	}

	return false;
}

extern bool
set_Value(set_t* set, intptr_t element, intptr_t* value) {
	uint32_t hash = hashElement(set, element);
	return internal_Value(set, hash, element, value);
}


extern bool
set_Exists(set_t* set, intptr_t element) {
	intptr_t value;
	return set_Value(set, element, &value);
}


extern void
set_Insert(set_t* set, intptr_t element) {
	assert(set != NULL);

	uint32_t hash = hashElement(set, element);
	SListEntry* list = &set->lists[hash];

	if (list->totalElements > 0 && list->elements != NULL) {
		for (uint32_t i = 0; i < list->totalElements; ++i) {
			if (list->elements[i] == element)
				return;

			if (set->equals(set->userData, list->elements[i], element)) {
				set->free(set->userData, list->elements[i]);
				list->elements[i] = element;
				return;
			}
		}
	}

	if (list->elements == NULL || list->allocatedElements == list->totalElements) {
		list->allocatedElements = list->allocatedElements * 2 + 4;
		list->elements = mem_Realloc(list->elements, list->allocatedElements * sizeof(void *));
	}
	list->elements[list->totalElements++] = element;
}


static bool
internal_Remove(set_t* set, uint32_t hash, intptr_t element) {
	assert(set != NULL);

	SListEntry* list = &set->lists[hash];

	if (list->totalElements > 0 && list->elements != NULL) {
		for (uint32_t i = 0; i < list->totalElements; ++i) {
			if (set->equals(set->userData, list->elements[i], element)) {
				set->free(set->userData, list->elements[i]);
				list->elements[i] = list->elements[--(list->totalElements)];
				return true;
			}
		}
	}

	for (uint32_t i; i < vec_Count(set->subSets); ++i) {
		if (internal_Remove((set_t*) vec_ElementAt(set->subSets, i), hash, element))
			return true;
	}

	return false;
}


extern void
set_Remove(set_t* set, intptr_t element) {
	uint32_t hash = hashElement(set, element);
	internal_Remove(set, hash, element);
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

	for (uint32_t i; i < vec_Count(set->subSets); ++i) {
		set_ForEachElement((set_t*) vec_ElementAt(set->subSets, i), forEach, data);
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
set_Clear(set_t* set) {
	assert(set != NULL);

	vec_Clear(set->subSets);

	for (uint32_t i = 0; i < SET_HASH_SIZE; ++i) {
		SListEntry* list = &set->lists[i];
		if (list->elements != NULL) {
			for (uint32_t j = 0; j < list->totalElements; ++j) {
				set->free(set->userData, list->elements[j]);
			}
			mem_Free(list->elements);
		}
		list->elements = NULL;
		list->allocatedElements = 0;
		list->totalElements = 0;
	}
}


extern void
set_Free(set_t* set) {
	assert(set != NULL);

	vec_Free(set->subSets);

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

static void
internal_ToArray(set_t* set, intptr_t* array, copy_t copy) {
	assert(set != NULL && array != NULL && copy != NULL);

	size_t arrayIndex = 0;
	for (uint32_t i = 0; i < SET_HASH_SIZE; ++i) {
		SListEntry* list = &set->lists[i];
		if (list->elements != NULL) {
			for (uint32_t j = 0; j < list->totalElements; ++j) {
				array[arrayIndex++] = copy(set->userData, list->elements[j]);
			}
		}
	}
	
	for (uint32_t i; i < vec_Count(set->subSets); ++i) {
		internal_ToArray((set_t*) vec_ElementAt(set->subSets, i), &array[arrayIndex], copy);
	}
}

extern intptr_t*
set_ToArray(set_t* set, copy_t copy, ssize_t* totalElements) {
	assert(set != NULL && copy != NULL && totalElements != NULL);

	*totalElements = set_Count(set);
	intptr_t* array = mem_Alloc(*totalElements * sizeof(intptr_t));

	internal_ToArray(set, array, copy);
	
	return array;
}

extern void
set_SetUserData(set_t* set, intptr_t data) {
	assert(set != NULL);
	set->userData = data;

	for (uint32_t i; i < vec_Count(set->subSets); ++i) {
		set_SetUserData((set_t*) vec_ElementAt(set->subSets, i), data);
	}

}

extern intptr_t
set_GetUserData(set_t* set) {
	assert(set != NULL);
	return set->userData;
}


extern set_t*
set_CreateSubSet(set_t* set) {
	set_t* subSet = set_Create(set->equals, set->hash, set->free);
	vec_PushBack(set->subSets, (intptr_t) subSet);
	return subSet;
}
