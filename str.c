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

#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "util.h"
#include "mem.h"
#include "str.h"
#include "strbuf.h"

typedef struct {
	uint32_t refCount;
	ssize_t length;
	char data[1];
} empty_string;

static empty_string g_emptyString = {
	1, 0, ""
};

static char
createSpace(void) {
	return ' ';
}

static void
copyOnWrite(string** str) {
	if ((*str)->refCount != 1) {
		string* newString = str_CreateLength((*str)->data, (*str)->length);
		str_Free(*str);
		*str = newString;
	}
}

static void
str_Set(string* str, ssize_t index, char ch) {
	if (index < 0)
		index = str_Length(str) + index;
	str->data[index] = ch;
}

INLINE string*
#if defined(_DEBUG)
str_AllocDebug(ssize_t length, const char* file, int lineNumber) {
	string* pString = mem_AllocImpl(sizeof(string) + length + 1, file, lineNumber);
#else
str_Alloc(ssize_t length) {
	string* pString = mem_Alloc(sizeof(string) + length + 1);
#endif
	pString->length = (uint32_t) length;
	pString->refCount = 1;
	return pString;
}

string*
#if defined(_DEBUG)
str_CreateLengthDebug(const char* data, ssize_t length, const char* filename, int lineNumber) {
	string* str = str_AllocDebug(length, filename, lineNumber);
#else
str_CreateLength(const char* data, ssize_t length) {
	string* str = str_Alloc(length);
#endif
	if (data != NULL) {
		memcpy(str->data, data, length);
	}
	str->data[length] = 0;
	return str;
}

string*
#if defined(_DEBUG)
str_CreateStreamDebug(char (*nextChar)(void), ssize_t length, const char* filename, int lineNumber) {
	string* str = str_AllocDebug(length, filename, lineNumber);
#else
str_CreateStream(char (*nextChar)(void), ssize_t length) {
	string* str = str_Alloc(length);
#endif
	for (ssize_t i = 0; i < length; ++i) {
		str_Set(str, i, nextChar());
	}
	str->data[length] = 0;
	return str;
}

string*
#if defined(_DEBUG)
str_CreateArgsDebug(const char* filename, int lineNumber, const char* format, va_list args) {
#else
str_CreateArgs(const char* format, va_list args) {
#endif
	string_buffer* buf = strbuf_Create();
	strbuf_AppendArgs(buf, format, args);

#if defined(_DEBUG)
	string* result = strbuf_StringDebug(buf, filename, lineNumber);
#else
	string* result = strbuf_String(buf);
#endif

	strbuf_Free(buf);
	return result;
}

string*
#if defined(_DEBUG)
str_CreateFormatDebug(const char* filename, int lineNumber, const char* format, ...) {
#else
str_CreateFormat(const char* format, ...) {
#endif
	va_list args;
	va_start(args, format);
#if defined(_DEBUG)
	string* result = str_CreateArgsDebug(filename, lineNumber, format, args);
#else
	string* result = str_CreateArgs(format, args);
#endif
	va_end(args);

	return result;
}

string*
str_Empty() {
	return str_Copy((string*) &g_emptyString);
}

void
str_Free(string* str) {
	if (str != NULL) {
		assert (str->refCount != 0);

		if (--str->refCount == 0)
			mem_Free(str);
	}
}

string*
#if defined(_DEBUG)
str_ConcatDebug(const string* str1, const string* str2, const char* file, int lineNumber) {
#else
str_Concat(const string* str1, const string* str2) {
#endif
	ssize_t length1 = str_Length(str1);
	ssize_t length2 = str_Length(str2);

	ssize_t newLength = length1 + length2;
#if defined(_DEBUG)
	string* newString = str_AllocDebug(newLength, file, lineNumber);
#else
	string* newString = str_Alloc(newLength);
#endif

	memcpy(newString->data, str_String(str1), length1);
	memcpy(&newString->data[length1], str_String(str2), length2 + 1);

	return newString;
}

string*
#if defined(_DEBUG)
str_SliceDebug(const string* str1, ssize_t index, ssize_t length, const char* file, int lineNumber) {
#else
str_Slice(const string* str1, ssize_t index, ssize_t length) {
#endif
	if (index < 0)
		index = str_Length(str1) + index;

	if (index >= (ssize_t) str_Length(str1))
		return str_Empty();

	if (index + length > str_Length(str1))
		length = str_Length(str1) - index;

#if defined(_DEBUG)
	return str_CreateLengthDebug(str_String(str1) + index, length, file, lineNumber);
#else
	return str_CreateLength(str_String(str1) + index, length);
#endif
}

uint32_t
str_Find(const string* haystack, const string* needle) {
	char* p = strstr(str_String(haystack), str_String(needle));
	if (p != NULL) {
		return (uint32_t)(p - str_String(haystack));
	} else {
		return UINT32_MAX;
	}
}

bool
str_Equal(const string* str1, const string* str2) {
	if (str1 == str2)
		return true;

	if (str1 == NULL || str2 == NULL)
		return false;
		
	ssize_t length1 = str_Length(str1);

	if (length1 != str_Length(str2))
		return false;

	for (ssize_t i = 0; i < length1; ++i) {
		if (str_CharAt(str1, i) != str_CharAt(str2, i))
			return false;
	}

	return true;
}

int
str_Compare(const string* str1, const string* str2) {
	const char* string1 = str_String(str1);
	const char* string2 = str_String(str2);

	while (*string1 && *string2) {
		uint8_t l1 = (uint8_t) *string1++;
		uint8_t l2 = (uint8_t) *string2++;

		if (l1 != l2)
			return l1 - l2;
	}

	return *string1 - *string2;
}

bool
str_EqualConst(const string* str1, const char* str2) {
	ssize_t length1 = str_Length(str1);
	char ch2;

	ch2 = *str2++;
	for (ssize_t i = 0; i < length1; ++i) {
		if (ch2 == 0 || str_CharAt(str1, i) != ch2)
			return false;

		ch2 = *str2++;
	}

	return ch2 == 0;
}

string*
#if defined(_DEBUG)
str_ReplaceDebug(const string* str, char search, char replace, const char* filename, int lineNumber) {
#else
str_Replace(const string* str, char search, char replace) {
#endif
	ssize_t length = str_Length(str);
#if defined(_DEBUG)
	string* result = str_CreateLengthDebug(str->data, length, filename, lineNumber);
#else
	string* result = str_CreateLength(str->data, length);
#endif

	for (ssize_t i = 0; i < length; ++i) {
		if (str_CharAt(result, i) == search)
			str_Set(result, i, replace);
	}

	return result;
}

extern string*
#if defined(_DEBUG)
str_ToLowerDebug(const string* str, const char* filename, int lineNumber) {
#define str_ToLower(str) str_ToLowerDebug(str, __FILE__, __LINE__)
#else
str_ToLower(const string* str) {
#endif
	ssize_t length = str_Length(str);
#if defined(_DEBUG)
	string* pLowerString = str_AllocDebug(length, filename, lineNumber);
#else
	string* pLowerString = str_Alloc(length);
#endif

	for (ssize_t i = 0; i < length; ++i) {
		str_Set(pLowerString, i, (char) tolower(str_CharAt(str, i)));
	}
	str_Set(pLowerString, length, 0);

	return pLowerString;
}

void
str_TransformReplace(string** str, char (*transform)(char)) {
	copyOnWrite(str);

	ssize_t len = str_Length(*str);
	for (ssize_t i = 0; i < len; ++i) {
		str_Set(*str, i, transform(str_CharAt(*str, i)));
	}
}

INLINE char charToUpper(char ch) {
	return (char) toupper(ch);
}

INLINE char charToLower(char ch) {
	return (char) tolower(ch);
}

void
str_ToUpperReplace(string** str) {
	str_TransformReplace(str, charToUpper);
}

void
str_ToLowerReplace(string** str) {
	str_TransformReplace(str, charToLower);
}

string*
#if defined(_DEBUG)
str_CreateSpacesDebug(uint32_t count, const char* filename, int lineNumber) {
	return str_CreateStreamDebug(createSpace, count, filename, lineNumber);
#else
str_CreateSpaces(uint32_t count) {
	return str_CreateStream(createSpace, count);
#endif
}

string*
str_Align(string* str, int32_t alignment) {
	int spaceCount = abs(alignment) - (int) str_Length(str);
	if (spaceCount > 0) {
		string* spacesString = str_CreateSpaces(spaceCount);
		string* aligned;
		if (alignment < 0) {
			aligned = str_Concat(str, spacesString);
		} else {
			aligned = str_Concat(spacesString, str);
		}
		str_Free(spacesString);
		return aligned;
	}

	return str_Copy(str);
}

extern uint32_t
str_JenkinsHashLength(const void* str, ssize_t length) {
	uint8_t* key = (uint8_t *) str;
	uint32_t hash = 0;
	for (ssize_t i = 0; i < length; ++i) {
		hash += key[i++];
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;

	return hash;
}

extern string*
#if defined(_DEBUG)
str_ReadFileDebug(FILE* fileHandle, size_t count, const char* filename, int lineNumber) {
	string* str = str_AllocDebug(count, filename, lineNumber);
#else
str_ReadFile(FILE* fileHandle, size_t count) {
	string* str = str_Alloc(count);
#endif
	count = fread(str->data, 1, count, fileHandle);
	str->data[count] = 0;
	return str;
}

extern string*
#if defined(_DEBUG)
str_CanonicalizeLineEndingsDebug(string* srcString, const char* filename, int lineNumber) {
	string* destString = str_AllocDebug(str_Length(srcString) + 1, filename, lineNumber);
#else
str_CanonicalizeLineEndings(string* srcString) {
	string* destString = str_Alloc(str_Length(srcString) + 1);
#endif
	char* src = srcString->data;
	char* dest = (char* )str_String(destString);
	while (src < srcString->data + srcString->length) {
		if ((src[0] == 10 && src[1] == 13) || (src[0] == 13 && src[1] == 10)) {
			*dest++ = '\n';
			src += 2;
		} else if (src[0] == 10 || src[0] == 13) {
			*dest++ = '\n';
			src += 1;
		} else {
			*dest++ = *src++;
		}
	}

	*dest++ = '\n';
	destString->length = (uint32_t) (dest - destString->data);

	return destString;
}
