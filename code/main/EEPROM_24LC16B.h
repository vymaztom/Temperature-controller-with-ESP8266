#ifndef EEPROM_24LC16B_H
#define EEPROM_24LC16B_H

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include "ProjectConfig.h"
#include "LList.h"

#define MAX_STRING 63

#define PAGE_1 0x50
#define PAGE_2 0x51
#define PAGE_3 0x52
#define PAGE_4 0x53
#define PAGE_5 0x54
#define PAGE_6 0x55
#define PAGE_7 0x56
#define PAGE_8 0x57


class EEPROM_24LC16B {

public:

	// Constructor
	EEPROM_24LC16B();

	// begin function call I2C Wire begin
	void begin();

	// Print page from EEPROM MEMORY
	void printPageMemory(uint8_t addr_i2c);

	void write(uint8_t addr_i2c, uint8_t* addr, char* data);

	char* read(uint8_t addr_i2c, uint8_t* addr);

	void write_ip(uint8_t addr_i2c, uint8_t* addr, uint8_t* data);

	uint8_t* read_ip(uint8_t addr_i2c, uint8_t* addr);

	// Read data from eeprom by address and selected i2c address
	void _eeprom_write(uint8_t chip_addr, uint8_t addr, uint8_t data);

	// Write data by address into eeprom selected by address
	uint8_t _eeprom_read(uint8_t chip_addr, uint8_t addr);

	uint8_t _eeprom_read_rec(uint8_t chip_addr, uint8_t addr, uint8_t deep, uint8_t maxdeep);

private:

	TwoWire myWire;

	uint8_t _eeprom_write_rec(uint8_t chip_addr, uint8_t addr, uint8_t data, uint8_t deep, uint8_t maxdeep);

};

#endif
