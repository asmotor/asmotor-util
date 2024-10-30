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

#if !defined(UTIL_PROTOS_H_INCLUDED_)
#define UTIL_PROTOS_H_INCLUDED_

#include <stdint.h>
#include <stdbool.h>


typedef intptr_t (*copy_t)(intptr_t userData, intptr_t element);
typedef bool (*equals_t)(intptr_t userData, intptr_t element1, intptr_t element2);
typedef uint32_t (*hash_t)(intptr_t userData, intptr_t element);
typedef void (*free_t)(intptr_t userData, intptr_t element);

#endif
