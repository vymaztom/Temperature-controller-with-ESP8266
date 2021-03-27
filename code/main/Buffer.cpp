#include "Buffer.h"


Buffer::Buffer(){
	_buffer[0] = '\0';
	pos = 0;
}

void Buffer::printf(const char* format, ...){
	va_list argptr;
    va_start(argptr, format);
	pos += vsprintf(_buffer + pos, format, argptr);
	va_end(argptr);
}

char* Buffer::get(){
	_buffer[pos] = '\0';
	return _buffer;
}
