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

#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif


#include "file.h"
#include "strbuf.h"

#if defined(WIN32)
#	include <direct.h>
#	define PATH_SEPARATOR '\\'
#	define PATH_REPLACE '/'
#	define getcwd _getcwd
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

void
ffill(uint8_t value, size_t count, FILE* fileHandle) {
	while (count--) {
		fputc(value, fileHandle);
	}
}

string*
fgetcwd(void) {
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        return str_Create(buffer);
    }
    return NULL;
}

string*
#if defined(_DEBUG)
fcanonicalizePathDebug(string* path, const char* filename, int lineNumber) {
	path = str_ReplaceDebug(path, PATH_REPLACE, PATH_SEPARATOR, filename, lineNumber);
#else
fcanonicalizePath(string* path) {
	path = str_Replace(path, PATH_REPLACE, PATH_SEPARATOR);
#endif
	const char* p = str_String(path);
	string_buffer* r = strbuf_Create();

	if (*p == PATH_SEPARATOR) {
		// Go to root
		strbuf_Free(r);
		r = strbuf_Create();
		strbuf_AppendChar(r, '/');
		++p;
	}

	while (*p != 0) {
		const char* e = strchr(p, PATH_SEPARATOR);
		if (e == NULL) {
			// Last component
			strbuf_AppendChars(r, p, strlen(p));
			p = p + strlen(p);
		} else if (e - p == 1 && strncmp(p, ".", 1) == 0) {
			// Ignore "."
			p = e + 1;
		} else if (e - p == 2 && strncmp(p, "..", 2) == 0) {
			if (strbuf_Size(r) == 0) {
				// Trying to go above root
				string* cwd = fgetcwd();
				if (cwd != NULL) {
					char* rend = strrchr(str_String(cwd), PATH_SEPARATOR);
					if (rend != NULL) {
						strbuf_AppendChars(r, str_String(cwd), rend - str_String(cwd) + 1);
					}
				}
				p = e + 1;
				continue;
			}
			// Remove last component from r
			if (strbuf_Size(r) == 1 && strbuf_Data(r)[0] == PATH_SEPARATOR) {
				// Already at root, ignore
				p = e + 1;
				continue;
			}
			if (strbuf_Size(r) > 0) {
				char* rend = strbuf_Data(r) + strbuf_Size(r) - 1;

				while (rend > strbuf_Data(r) && *(rend - 1) != PATH_SEPARATOR)
					--rend;

				strbuf_Truncate(r, (size_t) (rend - strbuf_Data(r)));
				p = e + 1;
			}
		} else {
			// Normal component
			if (e != p) {
				strbuf_AppendChars(r, p, e - p + 1);
			}
			p = e + 1;
		}
	}

	string* result = strbuf_String(r);
	strbuf_Free(r);

	return result;
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

