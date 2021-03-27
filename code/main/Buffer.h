#ifndef Buffer_h
#define Buffer_h

#include <Arduino.h>
#include <stdint.h>
#include <stdarg.h>

#define BUFFER_SIZE 1023


class Buffer {

public:
	Buffer();
	void printf(const char* format, ...);
	char* get();
private:
	char _buffer[BUFFER_SIZE];
	uint16_t pos;
};



#endif
