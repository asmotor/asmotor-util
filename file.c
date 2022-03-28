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

#include "file.h"
#include "strbuf.h"

#if defined(WIN32)
#	define PATH_SEPARATOR '\\'
#	define PATH_REPLACE '/'
#else
#	define PATH_SEPARATOR '/'
#	define PATH_REPLACE '\\'
#endif


size_t
fsize(FILE* fileHandle) {
	fflush(fileHandle);
    
    off_t currentOffset = ftello(fileHandle);
    fseeko(fileHandle, 0, SEEK_END);

	off_t size = ftello(fileHandle);
    fseeko(fileHandle, currentOffset, SEEK_SET);

    return (size_t) size;
}

void
fputll(uint32_t value, FILE* fileHandle) {
    fputc(value & 0xFFu, fileHandle);
    fputc((value >> 8u) & 0xFFu, fileHandle);
    fputc((value >> 16u) & 0xFFu, fileHandle);
    fputc((value >> 24u) & 0xFFu, fileHandle);
}

uint32_t
fgetll(FILE* fileHandle) {
    uint32_t r;

    r = (uint8_t) fgetc(fileHandle);
    r |= (uint32_t) fgetc(fileHandle) << 8u;
    r |= (uint32_t) fgetc(fileHandle) << 16u;
    r |= (uint32_t) fgetc(fileHandle) << 24u;

    return r;
}

void
fputbl(uint32_t value, FILE* fileHandle) {
    fputc((value >> 24u) & 0xFFu, fileHandle);
    fputc((value >> 16u) & 0xFFu, fileHandle);
    fputc((value >> 8u) & 0xFFu, fileHandle);
    fputc(value & 0xFFu, fileHandle);
}

uint32_t
fgetbl(FILE* fileHandle) {
    uint32_t r;

    r = (uint32_t) fgetc(fileHandle) << 24u;
    r |= (uint32_t) fgetc(fileHandle) << 16u;
    r |= (uint32_t) fgetc(fileHandle) << 8u;
    r |= (uint8_t) fgetc(fileHandle);

    return r;
}

uint16_t
fgetbw(FILE* fileHandle) {
    uint16_t hi = (uint16_t) fgetc(fileHandle) << 8u;
    return hi | (uint8_t) fgetc(fileHandle);
}

void
fputbw(uint16_t value, FILE* fileHandle) {
    fputc((uint8_t) (value >> 8u), fileHandle);
    fputc((uint8_t) value, fileHandle);
}

uint16_t
fgetlw(FILE* fileHandle) {
    uint16_t lo = (uint16_t) fgetc(fileHandle);
    return lo | (uint16_t) (fgetc(fileHandle) << 8u);
}

void
fputlw(uint16_t value, FILE* fileHandle) {
    fputc((uint8_t) value, fileHandle);
    fputc((uint8_t) (value >> 8u), fileHandle);
}

size_t
fgetsz(char* destination, size_t maxLength, FILE* fileHandle) {
    size_t r = 0;
    if (maxLength > 0) {
        char ch;

        do {
            ch = *destination++ = (char) fgetc(fileHandle);
            ++r;
        } while (r < maxLength && ch);
    }
    return r;
}

extern string*
fgetstr(FILE* fileHandle) {
    string_buffer* buf = strbuf_Create();

    while (true) {
        int ch = fgetc(fileHandle);
        if (ch == EOF || ch == 0)
            break;
        strbuf_AppendChar(buf, ch);
    }

    string* result = strbuf_String(buf);
    strbuf_Free(buf);
    return result;
}

extern void
fputsz(const char* str, FILE* fileHandle) {
    while (*str) {
        fputc(*str++, fileHandle);
    }
    fputc(0, fileHandle);
}

bool
fexists(const char* filename) {
    FILE* fileHandle = fopen(filename, "rb");
    if (fileHandle != NULL) {
        fclose(fileHandle);
        return true;
    }

    return false;
}

string*
#if defined(_DEBUG)
fcanonicalizePathDebug(string* path, const char* filename, int lineNumber) {
    return str_ReplaceDebug(path, PATH_REPLACE, PATH_SEPARATOR, filename, lineNumber);
#else
fcanonicalizePath(string* path) {
    return str_Replace(path, PATH_REPLACE, PATH_SEPARATOR);
#endif
}

string*
freplaceFileComponent(string* fullPath, string* fileName) {
    if (fullPath == NULL)
        return str_Copy(fileName);

    const char* lastSlash = str_String(fullPath) + str_Length(fullPath) - 1;

    while (lastSlash > str_String(fullPath) && *lastSlash != '/' && *lastSlash != '\\')
        --lastSlash;

    if (lastSlash == str_String(fullPath))
        return str_Copy(fileName);

    string* basePath = str_Slice(fullPath, 0, lastSlash + 1 - str_String(fullPath));
    string* newFullPath = str_Concat(basePath, fileName);
    str_Free(basePath);

    string* fixedPath = fcanonicalizePath(newFullPath);
    str_Free(newFullPath);
    return fixedPath;
}

