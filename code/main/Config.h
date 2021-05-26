#ifndef Config_h
#define Config_h

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include "LList.h"
#include "EEPROM_24LC16B.h"
#include "ProjectConfig.h"
#include "Buffer.h"

// SET MAX SIZE OF VALUSE
#define SIZE_NAMEDEVICE 32
#define SIZE_SSID 32
#define SIZE_PASSWORD 63

// INDEX OF VALUSE
#define I_NAME 0

#define I_AP_IP 1
#define I_AP_MASK 2
#define I_AP_GATEWAY 3
#define I_AP_SSID 4
#define I_AP_PASS 5
#define I_AP_NOTE 6

#define MAX_SCAN 10


typedef struct WiFiNote{
	char SSID[SIZE_SSID+1];
	char SECURE[20];
	int RSSI;
}WiFiNote_t;


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

	/************************ AP MODE SET METHODS ****************************/
	uint8_t setNAME(char* str);

	uint8_t setSSID(char* str);

	uint8_t setPASS(char* str);

	uint8_t setIP(char* str);

	uint8_t setMASK(char* str);

	uint8_t setGATEWAY(char* str);

	void _setIP(uint8_t* one);

	void _setMASK(uint8_t* one);

	void _setGATEWAY(uint8_t* one);

	/************************ AP MODE GET METHODS ****************************/

	// get device name
	char* getNAME();

	// get SSID ifnset return Notdefine
	char* getSSID();

	// get PASSWORD ifnset return Notdefine
	char* getPASS();

	// get IP ifnset return Notdefine
	char* getIP();

	// get MASK ifnset return Notdefine
	char* getMASK();

	// get GATEWAY ifnset return Notdefine
	char* getGATEWAY();

	// Buffer print of setted values into static section
	char* AP_Bprint();

	uint8_t* _getIP();

	uint8_t* _getMASK();

	uint8_t* _getGATEWAY();


	/**************************** STATION MODE *******************************/

	// add data into structure
	uint8_t STATION_addConnection(char* ssid, char* pass);

	// return Buffer CONSOLE format data of seted WIFI connections
	char* STATION_getConnectionCONSLE();

	// return Buffer JSON data of seted WIFI connections
	char* STATION_getConnectionJSON();

	// remove connection from structure by position
	void STATION_removeConnection(uint8_t index);

	void STATION_removeConnectionByName(char* str);

	uint8_t STATION_isInList(char* str);

	// return number of connections
	uint8_t STATION_getNumOfConnection();

	/**************************** TEXT VARIABLES *****************************/

	char* STATION_Bprint();


	void Print();

	/**************************** LOGGER DATA *****************************/
	void setTimerLogger(unsigned long input_seconds);

	unsigned long getTimerLogger();

	void setMyChanelNumber(unsigned long input);

	unsigned long getMyChanelNumber();

	void setApiKey(char* str);

	char* getApiKey();

	void setWifiMode(uint8_t value);

	uint8_t getWifiMode();

	void Save_buttons();

	void Load_buttons();

	uint8_t* StringToIP(char* str);

	char* IPToString(uint8_t* input);

	uint8_t WifiNets_index;
	WiFiNote_t WifiNets[MAX_SCAN];
	uint8_t controllerOnOff;
	uint8_t loggerOnOff;
	uint8_t dhcp[4];
	uint8_t selected_sensor;

	EEPROM_24LC16B eeprom;
	LList stationValues;

private:


	char name[SIZE_NAMEDEVICE+1];
	char ssid[SIZE_SSID+1];
	char password[SIZE_PASSWORD+1];
	uint8_t ip[4];
	uint8_t mask[4];
	uint8_t gateway[4];


	unsigned long TIMER_DIFFERENT_LOGER;
	unsigned long myChannelNumber;
	char myWriteAPIKey[30];
	uint8_t wifiMode;

	char output[17];

	// simple function for set static data into LList
	uint8_t _setByINDEX(uint8_t index, char* str);

	// secure function for printing undefined notes
	// retunr nondefine string if string is null
	char* _not2stringIfExist(char* one);

	// valide and decode string IP to array of uint8_t
	// retunr 5 x uint8_t
	// {(IP_1_byte),(IP_2_byte),(IP_3_byte),(IP_4_byte),(valide 1 or 0 for invalide)}

};




#endif
