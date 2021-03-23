#ifndef EEPROM_M24512_H
#define EEPROM_M24512_H

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>

// I2C address of the M24512
#define M24512_ADR 0x50

// size of string valus
#define MAXSIZEOFNAMEDEVICE 32
#define MAXSIZESSID 32
#define MAXSIZEWPA2PASSWORD 63

typedef struct config{
	char* name;
	char* ssid;
	char* password;
	uint8_t* ip;
	uint8_t* mask;
	uint8_t* gateway;
}config_t;


config_t* create_config();


// read data by address from M24512
uint8_t M24512_read_byte(uint16_t addr);

// write data at address into M24512
void M24512_write_byte(uint16_t addr, uint8_t data);

/******************************************************************************

				FUNCTIONS FOR SAVE AND LOAD CONFIG STRUCTURE

******************************************************************************/

// simple read uint8_t array from eeprom
void EEPROMread(uint8_t* _ret_data, uint8_t size, uint16_t addr);

// simple write uint8_t into eeprom
void EEPROMwrite(uint8_t* _data, uint8_t size, uint16_t addr);
// simple read char array from eeprom
void EEPROMread(char* _ret_data, uint8_t size, uint16_t addr);

// simple write uint8_t into eeprom
void EEPROMwrite(char* _data, uint8_t size, uint16_t addr);

// write structure into eeprom
config_t* read_config_eeprom();

// read strucure from eeprom
void write_config_eeprom(config_t* conf);



/*******************************************************************************

							dEBUG FUNCTIONS

*******************************************************************************/


void Config_print(config_t* one);


#endif
