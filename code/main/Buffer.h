#ifndef Buffer_h
#define Buffer_h

#include <Arduino.h>
#include <stdint.h>
#include <stdarg.h>

#define BUFFER_SIZE 2048


class Buffer {

public:
	Buffer();
	void printf(const char* format, ...);
	char* get();
	char* getMEMORY(uint16_t start, uint16_t size, char* strAdd);
	uint16_t size();
	void flush();
private:
	char _buffer[BUFFER_SIZE];
	uint16_t _pos;
	uint32_t _size;
};



#endif
