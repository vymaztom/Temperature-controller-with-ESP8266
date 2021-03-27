#ifndef Config_h
#define Config_h

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include "LList.h"
#include "EEPROM_M24512.h"
#include "ProjectConfig.h"
#include "Buffer.h"

// SET MAX SIZE OF VALUSE
#define SIZE_NAMEDEVICE 32
#define SIZE_SSID 32
#define SIZE_PASSWORD 63

// INDEX OF VALUSE
#define I_NAME 0

#define I_AP_IP 10
#define I_AP_MASK 11
#define I_AP_GATEWAY 12
#define I_AP_SSID 13
#define I_AP_PASS 14
#define I_AP_NOTE 15





class Config {

public:

	// Constructor of class Config
	Config();

	// Set pin modes
	void begin();

	// sava structures into eeprom
	void save();

	// load structures into eeprom
	void load();

/*
	static uint8_t validateIP(String ip);

	//static Vector<String> split(const char* str, char delim);

	static uint8_t isNumber(char* str);

*/


	/********************** GLOBAL VARIBLES METHODS **************************/

	uint8_t GLOBAL_setNAME(char* str);


	char* GLOBAL_getNAME();


	/************************ AP MODE SET METHODS ****************************/

	uint8_t AP_setSSID(char* str);

	uint8_t AP_setPASS(char* str);

	uint8_t AP_setIP(char* str);

	uint8_t AP_setMASK(char* str);

	uint8_t AP_setGATEWAY(char* str);

	uint8_t AP_setNOTE(char* str);

	/************************ AP MODE GET METHODS ****************************/

	// get SSID ifnset return Notdefine
	char* AP_getSSID();

	// get PASSWORD ifnset return Notdefine
	char* AP_getPASS();

	// get IP ifnset return Notdefine
	char* AP_getIP();

	// get MASK ifnset return Notdefine
	char* AP_getMASK();

	// get GATEWAY ifnset return Notdefine
	char* AP_getGATEWAY();

	char* AP_getNOTE();

	// Buffer print of setted values into static section
	char* AP_Bprint();


	/**************************** STATION MODE *******************************/

	// add data into structure
	uint8_t STATION_addConnection(char* ssid, char* pass);

	// return Buffer CONSOLE format data of seted WIFI connections
	char* STATION_getConnectionCONSLE();

	// return Buffer JSON data of seted WIFI connections
	char* STATION_getConnectionJSON();

	// remove connection from structure by position
	uint8_t STATION_removeConnection(uint8_t index);

	// return number of connections
	uint8_t STATION_getNumOfConnection();

	private:

		EEPROM_M24512 eeprom;
		LList staticValues;
		LList dinamicValues;

		// simple function for set static data into LList
		uint8_t _setByINDEX(uint8_t index, char* str);

		// simple function for get static data into LList
		char* _getByINDEX(uint8_t index);

		// secure function for printing undefined notes
		char* _not2stringIfExist(char* one);

	};




#endif
