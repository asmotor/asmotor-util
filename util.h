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

#ifndef UTIL_ASMOTOR_H_INCLUDED_
#define UTIL_ASMOTOR_H_INCLUDED_

#include <stdlib.h>

#if !defined(NDEBUG) && !defined(_DEBUG)
#   define _DEBUG
#endif

#if defined(__GNUC_STDC_INLINE__)
#	define INLINE static inline
#	define NORETURN(x) x __attribute__ ((noreturn))
#elif defined(_MSC_VER)
#	define INLINE static __inline
#	define NORETURN(x) __declspec(noreturn) x
#elif defined(__CALYPSI_CC__)
#	define INLINE static inline
#	define NORETURN(x) x __noreturn_function
#else
#	error "Unknown"
#	define INLINE static
#	define NORETURN(x) x
#endif

#if defined(_MSC_VER)
#   include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
#	include <unistd.h>
#endif

#if (defined(__VBCC__) || defined(__GNUC__)) && (!defined(__MINGW32__))
extern char*
_strdup(const char* str);

extern char*
_strupr(char* str);

extern char*
_strlwr(char* str);

extern int
_strnicmp(const char* string1, const char* string2, size_t length);

extern int
_stricmp(const char* string1, const char* string2);
#endif

#if defined(__GNUC__) && defined(__AMIGA__)
#define ftello ftell
#define fseeko fseek
#endif


#if defined(_MSC_VER) || defined(__VBCC__) || defined(__GNUC__)
# define internalerror(s) fprintf( stderr, "Internal error at "__FILE__"(%d): %s\n", __LINE__, s),exit(EXIT_FAILURE)
#else
# define internalerror(s) fprintf( stderr, "Internal error at "__FILE__"(%d): %s\n", __LINE__, s),exit(EXIT_FAILURE),return NULL
#endif

#endif /* UTIL_ASMOTOR_H_INCLUDED_ */
