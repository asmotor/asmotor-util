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

#include "mem.h"
#include "stream.h"


struct Stream {
	FILE* file;
	string* buffer;
	size_t position;
};


stream_t*
stream_CreateFromFilename(const char* name) {
	stream_t* stream = mem_Alloc(sizeof(stream_t));
	stream->file = fopen(name, "rt");
	stream->buffer = NULL;
	stream->position = 0;

	return stream;
}


stream_t*
stream_CreateFromString(const string* str) {
	stream_t* stream = mem_Alloc(sizeof(stream_t));
	stream->file = NULL;
	stream->buffer = str_Copy(str);
	stream->position = 0;

	return stream;
}


void
stream_Free(stream_t* stream) {
	if (stream == NULL)
		return;
		
	if (stream->file != NULL) {
		fclose(stream->file);
	} else if (stream->buffer != NULL) {
		str_Free(stream->buffer);
	}

	mem_Free(stream);
}


int
stream_GetChar(stream_t* stream) {
	if (stream->buffer) {
		if (stream->position < str_Length(stream->buffer))
			return str_CharAt(stream->buffer, stream->position++);
	} else if (stream->file) {
		return fgetc(stream->file);
	}
	return EOF;
}
