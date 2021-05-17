#include "Buffer.h"


Buffer::Buffer(){
	_buffer[0] = '\0';
	_pos = 0;
}

void Buffer::printf(const char* format, ...){
	va_list argptr;
	va_start(argptr, format);
	_pos += vsprintf(_buffer + _pos, format, argptr);
	va_end(argptr);
}

char* Buffer::get(){
	_buffer[_pos] = '\0';
	return _buffer;
}

char* Buffer::getMEMORY(uint16_t start, uint16_t size, char* strAdd){
	char* ret = (char*)malloc(sizeof(char)*(size+strlen(strAdd)+1));
	strncpy(ret, _buffer+start, size);
	ret[size+strlen(strAdd)] = '\0';
	return ret;
}


uint16_t Buffer::size(){
	return _pos;
}

void Buffer::flush(){
	_buffer[0] = '\0';
	_pos = 0;
}
