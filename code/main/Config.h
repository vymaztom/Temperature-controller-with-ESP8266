#ifndef WIFICONFIGJSON
#define WIFICONFIGJSON

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include <vector>
#include <String>
#include "eeprom_M24512.h"

// SET MAX SIZE OF VALUSE
#define SIZE_NAMEDEVICE 32
#define SIZE_SSID 32
#define SIZE_PASSWORD 63

// PIN DEFINITIONS
#define PIN_DATA 12
#define PIN_RELE 14
#define PIN_BUTTON 16
#define PIN_SCL 5
#define PIN_SDA 4
#define PIN_A0 A0

// INDEX OF VALUSE
#define I_NAME 0

#define I_AP_IP 10
#define I_AP_MASK 11
#define I_AP_GATEWAY 12
#define I_AP_SSID 13
#define I_AP_PASS 14

#define I_STATION_LIST 256

typedef struct node{
	uint8_t kind;
	uint8_t size;
	char* data;
	struct node* next;
}node_t;



class Config {

	private:

		node_t* _first_node;
		node_t* _last_node;
		uint8_t _size;

		void flush();

		uint8_t _add(uint8_t kind, char* data, uint8_t data_size);

		uint8_t add(uint8_t kind, char* data, uint8_t data_size);

		uint8_t add(uint8_t kind, const char* data, uint8_t data_size);
		// remove data from structure
		uint8_t remove(uint8_t kind);

		char* get(uint8_t kind);

		uint8_t getSize();

		bool _setByINDEX(uint8_t index, String& str);

		String _getByINDEX(uint8_t index);

	public:
		Config();

		void begin();

		void load();

		void save();

		void print();

		static bool validateIP(String ip);

		static vector<String> split(const String& str, char delim);

		static bool isNumber(const String& str);

		bool setDeviceName(String& str);

		// AP mode functions

		bool AP_setSSID(String& str);

		bool AP_setPASS(String& str);

		bool AP_setIP(String& str);

		bool AP_setMASK(String& str);

		bool AP_setGATEWAY(String& str);


		String AP_getSSID();

		String AP_getPASS();

		String AP_getIP();

		String AP_getMASK();

		String AP_getGATEWAY();

		// STATION MODE

		bool STATION_addConnection(String& ssid, String& pass);

		vector<pair<String,String>> STATION_getConnection();

		uint8_t STATION_getNumOfConnection();
};




#endif
