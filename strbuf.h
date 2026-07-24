/*  Copyright 2008-2026 Carsten Elton Sorensen

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

#pragma once

#include <stdarg.h>
#include <string.h>

#include "str.h"
#include "util.h"

typedef struct {
	size_t size;
	size_t allocated;
	char* data;
} string_buffer;

extern string_buffer*
strbuf_Create(void);

extern void
strbuf_Free(string_buffer* buffer);

INLINE size_t
strbuf_Size(string_buffer* buffer) {
	return buffer->size;
}

INLINE void
strbuf_Truncate(string_buffer* buffer, size_t newSize) {
	buffer->size = newSize;
}

INLINE char*
strbuf_Data(string_buffer* buffer) {
	return buffer->data;
}

extern string*
#if defined(_DEBUG)
strbuf_StringDebug(string_buffer* buffer, const char* filename, int lineNumber);
#define strbuf_String(buffer) strbuf_StringDebug(buffer, __FILE__, __LINE__)
#else
strbuf_String(string_buffer* buffer);
#endif

extern void
strbuf_AppendArgs(string_buffer* buffer, const char* format, va_list args);

extern void
strbuf_AppendFormat(string_buffer* buffer, const char* format, ...);

extern void
strbuf_AppendChars(string_buffer* buffer, const char* data, size_t length);

INLINE void
strbuf_AppendChar(string_buffer* buffer, char ch) {
	strbuf_AppendChars(buffer, &ch, 1);
}

INLINE void
strbuf_AppendStringZero(string_buffer* buffer, const char* str) {
	if (str == NULL)
		return;

	strbuf_AppendChars(buffer, str, strlen(str));
}

INLINE void
strbuf_AppendString(string_buffer* buffer, const string* str) {
	if (str == NULL)
		return;

	strbuf_AppendChars(buffer, str_String(str), str_Length(str));
}
