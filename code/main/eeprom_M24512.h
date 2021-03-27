#ifndef EEPROM_M24512_H
#define EEPROM_M24512_H

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include "ProjectConfig.h"
#include "LList.h"

// I2C address of the M24512
#define M24512_ADR 0x50
#define M24512_CLOCK 400000
//#define M24512_LIMIT 150000L 15ms
#define M24512_LIMIT 15000L
#define M24512_PAGE_SIZE 128
#define M24512_MAX_PAGES 500
#define M24512_PAGE_PRINT_LENGHT 16


class EEPROM_M24512 {

public:

	// Constructor
	EEPROM_M24512();

	// begin function call I2C Wire begin
	void begin();

	// Print page from EEPROM MEMORY
	void printPageMemory(uint16_t pageNumber);

	// save data from structure into EEPROM
	uint16_t M24512_saveLList(uint16_t offset, LList& list);

	// load data into structure from EEPROM
	uint16_t M24512_loadLList(uint16_t offset, LList& list);

//private:

	// read data by address from M24512
	uint8_t M24512_read_byte(uint16_t addr);

	// string data by address from M24512
	char* M24512_read_byte(uint16_t addr, uint8_t size);

	// write data at address into M24512
	void M24512_write_byte(uint16_t addr, uint8_t data);

	// write string data at address into M24512
	void M24512_write_byte(uint16_t addr, char* data);


	TwoWire myWire;

};

#endif
