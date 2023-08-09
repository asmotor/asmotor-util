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

#ifndef UTIL_FILE_H_INCLUDED_
#define UTIL_FILE_H_INCLUDED_

#include <stdio.h>

#include "util.h"
#include "str.h"
#include "types.h"

/* Open file securely */
INLINE FILE*
_util_fopen(const char* name, const char* mode) {
#if defined(_MSC_VER)
	FILE* f;
	if (fopen_s(&f, name, mode) == 0)
		return f;
	return NULL;
#else
	return fopen(name, mode);
#endif
}

#define fopen _util_fopen

/* Determine whether a file exists */
extern bool
fexists(const char* filename);

/* Return the size of a file in bytes */
extern size_t
fsize(FILE* fileHandle);

/* Write a little endian 32 bit value to a file */
extern void
fputll(uint32_t value, FILE* fileHandle);

/* Retrieve a little endian 32 bit value from a file */
extern uint32_t
fgetll(FILE* fileHandle);

/* Write a big endian 32 bit value to a file */
extern void
fputbl(uint32_t value, FILE* fileHandle);

/* Retrieve a big endian 32 bit value from a file */
extern uint32_t
fgetbl(FILE* fileHandle);

/* Retrieve a big endian 16 bit value from a file */
extern uint16_t
fgetbw(FILE* fileHandle);

/* Write a big endian 16 bit value to a file */
extern void
fputbw(uint16_t value, FILE* fileHandle);

/* Retrieve a little endian 16 bit value from a file */
extern uint16_t
fgetlw(FILE* fileHandle);

/* Write a little endian 16 bit value to a file */
extern void
fputlw(uint16_t value, FILE* fileHandle);

/* Retrieve a zero terminated string from a file */
extern size_t
fgetsz(char* destination, size_t maxLength, FILE* fileHandle);

/* Retrieve file contents as a string */
extern string*
fgetstr(FILE* fileHandle);

/* Write zero terminated string to a file */
extern void
fputsz(const char* str, FILE* fileHandle);

/* Write a number of bytes to a file */
extern void
ffill(uint8_t value, size_t count, FILE* fileHandle);

extern string*
#if defined(_DEBUG)
fcanonicalizePathDebug(string* path, const char* filename, int lineNumber);
#define fcanonicalizePath(path) fcanonicalizePathDebug(path, __FILE__, __LINE__)
#else
fcanonicalizePath(string* path);
#endif

/* Replace file name component from path */
extern string*
freplaceFileComponent(string* fullPath, string* fileName);


#ifdef __CALYPSI_TARGET_68000__
typedef int32_t off_t;

INLINE off_t ftello(FILE* fileHandle) {
	return ftell(fileHandle);
}

INLINE int fseeko(FILE* fileHandle, off_t offset, int origin) {
	return fseek(fileHandle, offset, origin);
}
#endif


#if defined(_MSC_VER)
typedef __int64 off_t;

INLINE off_t ftello(FILE* fileHandle) {
	return _ftelli64(fileHandle);
}

INLINE int fseeko(FILE* fileHandle, off_t offset, int origin) {
	return _fseeki64(fileHandle, offset, origin);
}
#endif

#endif /* UTIL_FILE_H_INCLUDED_ */
