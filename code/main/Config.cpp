#include "Config.h"


// constructor
Config::Config(){
}


void Config::begin(){
	eeprom.begin();
	pinMode(PIN_RELE, OUTPUT);
	pinMode(PIN_BUTTON, INPUT);
}

/*
// load data form EEPROM
void Config::load(){
	flush();
	uint16_t addr = 0;
	uint8_t struct_size = M24512_read_byte(addr);
	addr++;
	for(uint8_t i = 0 ; i < struct_size ; i++){
		uint8_t kind = (uint8_t)M24512_read_byte(addr);
		addr++;
		uint8_t size = M24512_read_byte(addr);
		addr++;
		char* data = (char*)malloc(sizeof(char)*size);
		for(uint8_t j = 0 ; j < size ; j++){
			data[j] = (char)M24512_read_byte(addr);
			addr++;
		}
		_add(kind, data, size);
	}

}

// save data into eeprom
void Config::save(){
	uint16_t addr = 0;
	M24512_write_byte(addr, _size);
	addr++;
	if(_size > 0){
		node_t* one = _first_node;
		while(one != NULL){
			M24512_write_byte(addr, (uint8_t)one->kind);
			addr++;
			M24512_write_byte(addr, (uint8_t)one->size);
			addr++;
			for(uint8_t i = 0 ; i < one->size ; i++){
				M24512_write_byte(addr, (uint8_t)one->data[i]);
				addr++;
			}
			one = one->next;
		}
	}
}
*/
/******************************************************************************

					STATIC FUNCTONS FOR DECODE IP FROM TEXT

******************************************************************************/
/*

uint8_t Config::isNumber(char* str){
	uint8_t isNum = false;
	if(str != ""){
		char numbers[] = "0123456789";
		isNum = true;
		for(uint8_t i ; i < strlen(str) ; i++){
			uint8_t find = false;
			for(uint8_t j ; j < 10 ; j++){
				if(numbers[j] == str[i]){
					find = true;
					break;
				}
			}
			isNum = isNum && find;
		}
	}
    return isNum;
}

/*
Vector<String> Config::split(const char* str, char delim){
    auto i = 0;
    Vector<String> list;

    auto pos = str.find(delim);

    while (pos != String::npos) {
        list.push_back(str.substr(i, pos - i));
        i = ++pos;
        pos = str.find(delim, pos);
    }

    list.push_back(str.substr(i, str.length()));

    return list;
}


uint8_t Config::validateIP(String ip){
/*
	Vector<String> list = split(ip, '.');

	if(list.size() != 4){
		return false;
	}

	for(String str : list){
		if (!isNumber(str) || stoi(str) > 255 || stoi(str) < 0)
		return false;
	}

	return true;
}
*/

/******************************************************************************

	FUNCTIONS FOR Static Variable ADD and GET DATA INTO LList STRUCTURE

******************************************************************************/

// simple function for set static data into LList
uint8_t Config::_setByINDEX(uint8_t index, char* str){
	staticValues.remove(index);
	return staticValues.add(index, str);
}

// simple function for get static data into LList
char* Config::_getByINDEX(uint8_t index){
	char* one = staticValues.get(index);
	return _not2stringIfExist(one);
}


char* Config::_not2stringIfExist(char* one){
	if(one != NULL){
		return one;
	}
	return "Notdefine";
}


// GLOBAL VARIBALES

uint8_t Config::GLOBAL_setNAME(char* str){
	return _setByINDEX(I_NAME, str);
}

char* Config::GLOBAL_getNAME(){
	return _getByINDEX(I_NAME);
}



// AP mode functions

uint8_t Config::AP_setSSID(char* str){
	return _setByINDEX(I_AP_SSID, str);
}

uint8_t Config::AP_setPASS(char* str){
	return _setByINDEX(I_AP_PASS, str);
}

uint8_t Config::AP_setIP(char* str){
	return _setByINDEX(I_AP_IP, str);
}

uint8_t Config::AP_setMASK(char* str){
	return _setByINDEX(I_AP_MASK, str);
}

uint8_t Config::AP_setGATEWAY(char* str){
	return _setByINDEX(I_AP_GATEWAY, str);
}

uint8_t Config::AP_setNOTE(char* str){
	return _setByINDEX(I_AP_NOTE, str);
}


char* Config::AP_getSSID(){
	return _getByINDEX(I_AP_SSID);
}

char* Config::AP_getPASS(){
	return _getByINDEX(I_AP_PASS);
}

char* Config::AP_getIP(){
	return _getByINDEX(I_AP_IP);
}

char* Config::AP_getMASK(){
	return _getByINDEX(I_AP_MASK);
}

char* Config::AP_getGATEWAY(){
	return _getByINDEX(I_AP_GATEWAY);
}

char* Config::AP_getNOTE(){
	return _getByINDEX(I_AP_NOTE);
}


char* Config::AP_Bprint(){
	Buffer buffer;
	buffer.printf("NAME: %s \n", GLOBAL_getNAME());
	buffer.printf("SSID: %s \n", AP_getSSID());
	buffer.printf("PASS: %s \n", AP_getPASS());
	buffer.printf("  IP: %s \n", AP_getIP());
	buffer.printf("MASK: %s \n", AP_getMASK());
	buffer.printf("GATE: %s \n", AP_getGATEWAY());
	buffer.printf("NOTE: %s \n", AP_getNOTE());
	return buffer.get();
}

/******************************************************************************

							STATION MODE

******************************************************************************/

// add data into structure
uint8_t Config::STATION_addConnection(char* ssid, char* pass){
	dinamicValues.removeByValue1(ssid);
	uint8_t max = dinamicValues.getMaxIndex();
	if(max < 255){
		return dinamicValues.add(max+1, ssid, pass);
	}
	return 0;
}

// return Buffer CONSOLE format data of seted WIFI connections
char* Config::STATION_getConnectionCONSLE(){
	Buffer buffer;
	buffer.printf("SETTED WIFI FOR CONNECTION\n");
	for(uint8_t i = 0 ; i <= dinamicValues.getMaxIndex() ; i++){
		note_t* one = dinamicValues.get_note(i);
		if(one != NULL){
			buffer.printf("ID:%3u [SSID: %12s ; password(WPA2): %12s]\n", i, one->value1, one->value2);
		}
	}
	return buffer.get();
}

// return Buffer JSON data of seted WIFI connections
char* Config::STATION_getConnectionJSON(){
	Buffer buffer;
	buffer.printf("[");
	for(uint8_t i = 0 ; i <= dinamicValues.getMaxIndex() ; i++){
		note_t* one = dinamicValues.get_note(i);
		if(one != NULL){
			buffer.printf("{");
			buffer.printf("\"ID\": %i , \"SSID\" : \"%s\"", i, one->value1);
			buffer.printf("}");
		}
	}
	buffer.printf("]");
	return buffer.get();
}

// remove connection from structure by position
uint8_t Config::STATION_removeConnection(uint8_t index){
	dinamicValues.remove(index);
}

// return number of connections
uint8_t Config::STATION_getNumOfConnection(){
	return dinamicValues.getSize();
}



/******************************************************************************

						SAVE LOAD FUNCTIONS EEPROM_M24512

******************************************************************************/

// sava structures into eeprom
void Config::save(){
	uint16_t offset = 0;
	offset = eeprom.M24512_saveLList(offset, staticValues);
	offset = eeprom.M24512_saveLList(offset, dinamicValues);
	eeprom.printPageMemory(0);
	eeprom.printPageMemory(1);
}

// load structures into eeprom
void Config::load(){
	uint16_t offset = 0;
	offset = eeprom.M24512_loadLList(offset, staticValues);
	offset = eeprom.M24512_loadLList(offset, dinamicValues);
}
