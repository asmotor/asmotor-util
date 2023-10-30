#include "strpmap.h"

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
map_foreach(intptr_t key, intptr_t value, intptr_t data) {
	foreach_data_t* d = (foreach_data_t*) data;
	d->forEach((const char*) key, value, d->data);
}

extern void
strpmap_ForEachKeyValue(strpmap_t* map, strpmap_foreach_t forEach, intptr_t data) {
	foreach_data_t fd = { forEach, data };
	map_ForEachKeyValue((map_t*) map, map_foreach, (intptr_t) &fd);
}



