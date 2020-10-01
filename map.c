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

#define IN_MAP_C_

#include "set.h"

typedef struct {
    equals_t keyEquals;
    hash_t keyHash;
    free_t keyFree;
    free_t valueFree;
    set_t* set;
    intptr_t userData;
} map_t;

#include "map.h"


// Internal structures

typedef struct {
    intptr_t key;
    intptr_t value;
} keyvalue_t;

typedef struct {
    map_foreach_t forEach;
    intptr_t data;
} setcallbackdata_t;

static bool
keyvalueEquals(intptr_t userData, intptr_t element1, intptr_t element2) {
    map_t* map = (map_t*) userData;
    return map->keyEquals(map->userData, ((keyvalue_t*) element1)->key, ((keyvalue_t*) element2)->key);
}

static uint32_t
keyvalueHash(intptr_t userData, intptr_t element) {
    map_t* map = (map_t*) userData;
    keyvalue_t* kv = (keyvalue_t*) element;
    return map->keyHash(map->userData, kv->key);
}

static void
keyvalueFree(intptr_t userData, intptr_t element) {
    map_t* map = (map_t*) userData;
    keyvalue_t* kv = (keyvalue_t*) element;
    map->keyFree(map->userData, kv->key);
    map->valueFree(map->userData, kv->value);
    mem_Free(kv);
}

static bool
keyPredicate(intptr_t userData, intptr_t key, intptr_t element) {
    map_t* map = (map_t*) userData;
    keyvalue_t* kv = (keyvalue_t*) element;
    return map->keyEquals(map->userData, kv->key, key);
}

static void
forEachSetElement(intptr_t element, intptr_t data) {
    setcallbackdata_t* mapData = (setcallbackdata_t*) data;
    keyvalue_t* keyValue = (keyvalue_t*) element;
    mapData->forEach(keyValue->key, keyValue->value, mapData->data);
}


// Public functions

extern map_t*
#if defined(_DEBUG)
map_CreateDebug(equals_t keyEquals, hash_t keyHash, free_t keyFree, free_t valueFree, const char* filename, int lineNumber) {
    map_t* map = (map_t*) mem_AllocImpl(sizeof(map_t), filename, lineNumber);
#else
map_Create(equals_t keyEquals, hash_t keyHash, free_t keyFree, free_t valueFree) {
    map_t* map = (map_t*) mem_Alloc(sizeof(map_t));
#endif
    map->set = set_Create(keyvalueEquals, keyvalueHash, keyvalueFree);
    map->keyEquals = keyEquals;
    map->keyHash = keyHash;
    map->keyFree = keyFree;
    map->valueFree = valueFree;
    set_SetUserData(map->set, (intptr_t) map);

    return map;
}

extern void
map_Free(map_t* map) {
	set_Free(map->set);
	mem_Free(map);
}

extern void
map_Insert(map_t* map, intptr_t key, intptr_t value) {
    keyvalue_t* kv = (keyvalue_t*) mem_Alloc(sizeof(keyvalue_t));
    kv->key = key;
    kv->value = value;
    set_Remove(map->set, (intptr_t) kv);
    set_Insert(map->set, (intptr_t) kv);
}

extern bool
map_Value(map_t* map, intptr_t key, intptr_t* value) {
    intptr_t element;
    if (set_Find(map->set, keyPredicate, key, &element)) {
        keyvalue_t* kv = (keyvalue_t*) element;
        *value = kv->value;
        return true;
    }
    return false;
}

extern bool
map_HasKey(map_t* map, intptr_t key) {
    intptr_t element;
    return set_Find(map->set, keyPredicate, key, &element);
}

extern void
map_ForEachKeyValue(map_t* map, map_foreach_t forEach, intptr_t data) {
    setcallbackdata_t mapData = { forEach, data };
    set_ForEachElement(map->set, forEachSetElement, (intptr_t) &mapData);
}

extern ssize_t
map_Count(map_t* map) {
    return set_Count(map->set);
}
