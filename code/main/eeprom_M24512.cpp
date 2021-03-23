#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include "eeprom_M24512.h"


config_t* create_config(){
	config_t* one = (config_t*)malloc(sizeof(config_t));
	one->name = (char*)malloc(sizeof(char)*MAXSIZEOFNAMEDEVICE);
	one->ssid = (char*)malloc(sizeof(char)*MAXSIZESSID);
	one->password = (char*)malloc(sizeof(char)*MAXSIZEWPA2PASSWORD);
	one->ip = (uint8_t*)malloc(sizeof(uint8_t)*4);
	one->mask = (uint8_t*)malloc(sizeof(uint8_t)*4);
	one->gateway = (uint8_t*)malloc(sizeof(uint8_t)*4);
	return one;
}


/******************************************************************************

			PART OF FUNCTIONS FOR COMUNICATION WITH EEPROM M24512

*******************************************************************************/

// read data by address from M24512
uint8_t M24512_read_byte(uint16_t addr){
	Wire.beginTransmission(M24512_ADR);
	Wire.write(addr >> 8);
	Wire.write(addr & 0xFF);
	Wire.endTransmission();
	Wire.requestFrom(M24512_ADR, 1);
	if(Wire.available() == 1){
		return Wire.read();
	}
	return 0xF;
}


// write data at address into M24512
void M24512_write_byte(uint16_t addr, uint8_t data){
	Wire.beginTransmission(M24512_ADR);
	Wire.write(addr >> 8);
	Wire.write(addr & 0xFF);
	Wire.write(data);
	Wire.endTransmission();
	delay(10);
}

/******************************************************************************

				FUNCTIONS FOR SAVE AND LOAD CONFIG STRUCTURE

******************************************************************************/

// simple read uint8_t array from eeprom
void EEPROMread(uint8_t* _ret_data, uint8_t size, uint16_t addr){
	for(uint8_t i = 0; i < size; i++){
		_ret_data[i] = M24512_read_byte(addr + i);
	}
}

// simple write uint8_t into eeprom
void EEPROMwrite(uint8_t* _data, uint8_t size, uint16_t addr){
	for(uint8_t i = 0; i < size; i++){
		M24512_write_byte(addr + i, _data[i]);
	}
}

// simple read char array from eeprom
void EEPROMread(char* _ret_data, uint8_t size, uint16_t addr){
	for(uint8_t i = 0; i < size; i++){
		_ret_data[i] = (char)M24512_read_byte(addr + i);
	}
}

// simple write uint8_t into eeprom
void EEPROMwrite(char* _data, uint8_t size, uint16_t addr){
	for(uint8_t i = 0; i < size; i++){
		M24512_write_byte(addr + i, (uint8_t)_data[i]);
	}
}


// write structure into eeprom
config_t* read_config_eeprom(){
	config_t* ret = create_config();
	uint16_t addr = 0;
	EEPROMread(ret->name, MAXSIZEOFNAMEDEVICE, addr);
	addr += MAXSIZEOFNAMEDEVICE;
	EEPROMread(ret->ssid, MAXSIZESSID, addr);
	addr += MAXSIZESSID;
	EEPROMread(ret->password, MAXSIZEWPA2PASSWORD, addr);
	addr += MAXSIZEWPA2PASSWORD;
	EEPROMread(ret->ip, 4, addr);
	addr += 4;
	EEPROMread(ret->mask, 4, addr);
	addr += 4;
	EEPROMread(ret->gateway, 4, addr);
	return ret;
}

// read strucure from eeprom
void write_config_eeprom(config_t* conf){
	uint16_t addr = 0;
	EEPROMwrite(conf->name, MAXSIZEOFNAMEDEVICE, addr);
	addr += MAXSIZEOFNAMEDEVICE;
	EEPROMwrite(conf->ssid, MAXSIZESSID, addr);
	addr += MAXSIZESSID;
	EEPROMwrite(conf->password, MAXSIZEWPA2PASSWORD, addr);
	addr += MAXSIZEWPA2PASSWORD;
	EEPROMwrite(conf->ip, 4, addr);
	addr += 4;
	EEPROMwrite(conf->mask, 4, addr);
	addr += 4;
	EEPROMwrite(conf->gateway, 4, addr);
}


/*******************************************************************************

							dEBUG FUNCTIONS

*******************************************************************************/

void Config_print(config_t* one){
	Serial.println(">>>> Print config");
	Serial.print("Name: ");
	Serial.println(one->name);
	Serial.print("SSID: ");
	Serial.println(one->ssid);
	Serial.print("password: ");
	Serial.println(one->password);

	Serial.print("ip: ");
	Serial.print(one->ip[0]);
	Serial.print(".");
	Serial.print(one->ip[1]);
	Serial.print(".");
	Serial.print(one->ip[2]);
	Serial.print(".");
	Serial.print(one->ip[3]);
	Serial.println("");

	Serial.print("ip: ");
	Serial.print(one->mask[0]);
	Serial.print(".");
	Serial.print(one->mask[1]);
	Serial.print(".");
	Serial.print(one->mask[2]);
	Serial.print(".");
	Serial.print(one->mask[3]);
	Serial.println("");

	Serial.print("ip: ");
	Serial.print(one->gateway[0]);
	Serial.print(".");
	Serial.print(one->gateway[1]);
	Serial.print(".");
	Serial.print(one->gateway[2]);
	Serial.print(".");
	Serial.print(one->gateway[3]);
	Serial.println("");

}
