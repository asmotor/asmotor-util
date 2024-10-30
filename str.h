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

#if !defined(UTIL_STR_H_INCLUDED_)
#define UTIL_STR_H_INCLUDED_

#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "util.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable: 4200)
#endif

typedef struct {
	uint32_t refCount;
	uint32_t length;
	char data[];
} string;

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#if defined(_MSC_VER)
#   define strncpy(dest, src, len) strncpy_s(dest, len, src, len)
#endif

extern string*
#if defined(_DEBUG)
str_CreateLengthDebug(const char* data, size_t length, const char* file, int lineNumber);
#define str_CreateLength(data, length) str_CreateLengthDebug(data, length, __FILE__, __LINE__)
#else
str_CreateLength(const char* data, size_t length);
#endif

extern string*
#if defined(_DEBUG)
str_CreateStreamDebug(char (*nextChar)(void), size_t length, const char* filename, int lineNumber);
#define str_CreateStream(nextChar, length) str_CreateStreamDebug(nextChar, length, __FILE__, __LINE__)
#else
str_CreateStream(char (*nextChar)(void), size_t length);
#endif

extern string*
#if defined(_DEBUG)
str_CreateSpacesDebug(uint32_t count, const char* filename, int lineNumber);
#define str_CreateSpaces(count) str_CreateSpacesDebug(count, __FILE__, __LINE__)
#else
str_CreateSpaces(uint32_t count);
#endif

extern string*
#if defined(_DEBUG)
str_CreateArgsDebug(const char* filename, int lineNumber, const char* format, va_list args);
#define str_CreateArgs(format, args) str_CreateArgsDebug(__FILE__, __LINE__, format, args)
#else
str_CreateArgs(const char* format, va_list args);
#endif

extern string*
#if defined(_DEBUG)
str_CreateFormatDebug(const char* filename, int lineNumber, const char* format, ...);
#define str_CreateFormat(format, ...) str_CreateFormatDebug(__FILE__, __LINE__, format, __VA_ARGS__)
#else
str_CreateFormat(const char* format, ...);
#endif

INLINE string*
#if defined(_DEBUG)
str_CreateDebug(const char* data, const char* filename, int lineNumber) {
	return str_CreateLengthDebug(data, strlen(data), filename, lineNumber);
#define	str_Create(data) str_CreateDebug(data, __FILE__, __LINE__)
#else
str_Create(const char* data) {
	return str_CreateLength(data, strlen(data));
#endif
}

extern string*
str_Empty(void);

extern void
str_Free(string* str);

extern string*
#if defined(_DEBUG)
str_ConcatDebug(const string* str1, const string* str2, const char* file, int lineNumber);
#define str_Concat(str1, str2) str_ConcatDebug(str1, str2, __FILE__, __LINE__)
#else
str_Concat(const string* str1, const string* str2);
#endif

extern string*
#if defined(_DEBUG)
str_SliceDebug(const string* str1, ssize_t index, ssize_t length, const char* file, int lineNumber);
#define str_Slice(str1, index, length) str_SliceDebug(str1, index, length, __FILE__, __LINE__)
#else
str_Slice(const string* str1, ssize_t index, ssize_t length);
#endif

extern uint32_t
str_Find(const string* haystack, const string* needle);

extern uint32_t
str_FindChar(const string* haystack, char needle);

extern bool
str_Equal(const string* str1, const string* str2);

extern int
str_Compare(const string* str1, const string* str2);

extern bool
str_EqualConst(const string* str1, const char* str2);

extern string*
#if defined(_DEBUG)
str_ReplaceDebug(const string* str, char search, char replace, const char* filename, int lineNumber);
#define str_Replace(str, search, replace) str_ReplaceDebug(str, search, replace, __FILE__, __LINE__)
#else
str_Replace(const string* str, char search, char replace);
#endif

extern string*
#if defined(_DEBUG)
str_ToLowerDebug(const string* str, const char* file, int lineNumber);
#define str_ToLower(str) str_ToLowerDebug(str, __FILE__, __LINE__)
#else
str_ToLower(const string* str);
#endif

extern void
str_TransformReplace(string** str, char (*transform)(char));

extern void
str_ToUpperReplace(string** str);

extern void
str_ToLowerReplace(string** str);

extern string*
str_Align(string* str, int32_t alignment);

INLINE bool
str_NotEqual(const string* str1, const string* str2) {
	return !str_Equal(str1, str2);
}

INLINE size_t
str_Count(const string* str, char ch) {
	size_t count = 0;
	for (size_t i = 0; i < str->length; ++i) {
		if (str->data[i] == ch)
			count += 1;
	}
	return count;
}

INLINE string*
str_Copy(const string* str) {
	if (str != NULL)
		++((string*) str)->refCount;
	return (string*) str;
}

INLINE size_t
str_Length(const string* str) {
	return str->length;
}

INLINE const char*
str_String(const string* str) {
	assert(str != NULL);
	return str->data;
}

INLINE char
str_CharAt(const string* str, ssize_t index) {
	assert(str != NULL);
	if (index < 0)
		index = str_Length(str) + index;
	return str->data[index];
}

INLINE void
str_Assign(string** dest, const string* src) {
	str_Free(*dest);
	*dest = str_Copy(src);
}

INLINE void
str_Move(string** dest, string** src) {
	str_Free(*dest);
	*dest = *src;
	*src = NULL;
}

extern uint32_t
str_JenkinsHashLength(const void* str, size_t length);

INLINE uint32_t
str_JenkinsHash(const string* str) {
	return str_JenkinsHashLength(str_String(str), str_Length(str));
}

extern uint32_t
str_JenkinsHashLengthI(const void* str, size_t length);

INLINE uint32_t
str_JenkinsHashI(const string* str) {
	return str_JenkinsHashLengthI(str_String(str), str_Length(str));
}

extern string*
#if defined(_DEBUG)
str_ReadFileDebug(FILE* fileHandle, size_t count, const char* file, int lineNumber);
#define str_ReadFile(file, count) str_ReadFileDebug(file, count, __FILE__, __LINE__)
#else
str_ReadFile(FILE* fileHandle, size_t count);
#endif

extern string*
#if defined(_DEBUG)
str_CanonicalizeLineEndingsDebug(string* srcString, const char* file, int lineNumber);
#define str_CanonicalizeLineEndings(srcString) str_CanonicalizeLineEndingsDebug(srcString, __FILE__, __LINE__);
#else
str_CanonicalizeLineEndings(string* srcString);
#endif

INLINE bool
hexToInt(const char* text, uint32_t* result) {
#if defined(_MSC_VER)
	int count = sscanf_s(text, "%x", result);
#else
	int count = sscanf(text, "%x", result);
#endif
	return count == 1;
}

INLINE bool
decimalToInt(const char* text, int32_t* result) {
#if defined(_MSC_VER)
	int count = sscanf_s(text, "%d", result);
#else
	int count = sscanf(text, "%d", result);
#endif
	return count == 1;
}

#define STR_ASSIGN(p, str) str_Assign(&(p), (str))
#define STR_MOVE(p, str)   str_Move(&(p), &(str))

#endif
