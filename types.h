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

#ifndef UTIL_TYPES_H_INCLUDED_
#define UTIL_TYPES_H_INCLUDED_

#include <stdint.h> // IWYU pragma: export
#include <stddef.h> // IWYU pragma: export
#include <stdbool.h> // IWYU pragma: export

#ifndef NULL
# define NULL 0L
#endif

#if _POSIX_VERSION < 200809L
#	if SIZE_MAX==4294967295u
typedef int32_t ssize_t;
#	elif SIZE_MAX==18446744073709551615ul
typedef int64_t ssize_t;
#	else
#		error "Unknown size_t size"
#	endif
#endif


#endif /* UTIL_TYPES_H_INCLUDED_ */
