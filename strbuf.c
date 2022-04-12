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

#include <stdarg.h>

#include "mem.h"
#include "strbuf.h"

#define INITIAL_SIZE 32U

string_buffer*
strbuf_Create(void) {
    string_buffer* buffer = mem_Alloc(sizeof(string_buffer));
    buffer->size = 0;
    buffer->data = mem_Alloc(buffer->allocated = INITIAL_SIZE);

    return buffer;
}

void
strbuf_Free(string_buffer* buffer) {
    mem_Free(buffer->data);
    mem_Free(buffer);
}

string*
#if defined(_DEBUG)
strbuf_StringDebug(string_buffer* buffer, const char* filename, int lineNumber) {
    return str_CreateLengthDebug(buffer->data, buffer->size, filename, lineNumber);
#else
strbuf_String(string_buffer* buffer) {
    return str_CreateLength(buffer->data, buffer->size);
#endif
}

void
strbuf_AppendChars(string_buffer* buffer, const char* data, size_t length) {
    if (data == NULL)
        return;

    if (length + buffer->size > buffer->allocated) {
        size_t newSize = length + buffer->size;
        newSize += newSize >> 1u;

        buffer->data = mem_Realloc(buffer->data, newSize);
        buffer->allocated = newSize;
    }

    memcpy(buffer->data + buffer->size, data, length);
    buffer->size += length;
}

void
strbuf_AppendArgs(string_buffer* buffer, const char* format, va_list args) {
    int sz = 80;
    int nsz;
    char* mem = mem_Alloc(80);
    while (true) {
        nsz = vsnprintf(mem, sz, format, args);
        if (nsz >= 0 && nsz < sz)
            break;
        mem = mem_Realloc(mem, nsz + 1);
        sz = nsz + 1;
    }
    strbuf_AppendChars(buffer, mem, nsz);
    mem_Free(mem);
}

extern void
strbuf_AppendFormat(string_buffer* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);
    strbuf_AppendArgs(buffer, format, args);
    va_end(args);
}
