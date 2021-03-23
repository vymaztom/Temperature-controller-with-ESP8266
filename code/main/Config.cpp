#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include "eeprom_M24512.h"
#include "Config.h"


// constructor
Config::Config(){
	_size = 0;
	_last_node = NULL;
	_first_node = NULL;
}


void Config::begin(){
	Wire.begin(PIN_SDA, PIN_SCL);
	pinMode(PIN_RELE, OUTPUT);
	pinMode(PIN_BUTTON, INPUT);
}



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


// add data into structure
uint8_t Config::_add(uint8_t kind, char* data, uint8_t data_size){
	node_t* one = (node_t*)malloc(sizeof(node_t));
	if(one == NULL){
		return 0;
	}

	one->data = data;
	one->kind = kind;
	one->size = data_size;
	one->next = NULL;

	if(_size == 0){
		_first_node = one;
		_last_node = one;
		_size++;
	}else{
		_last_node->next = one;
		_last_node = one;
		_size++;
	}

	return 1;
}

uint8_t Config::add(uint8_t kind, char* data, uint8_t data_size){
	node_t* one = (node_t*)malloc(sizeof(node_t));
	if(one == NULL){
		return 0;
	}
	one->data = (char*)malloc(sizeof(char)*data_size);
	if(one == NULL){
		free(one);
		return 0;
	}
	for(uint8_t i = 0 ; i < data_size ; i++){
		one->data[i] = data[i];
	}

	one->kind = kind;
	one->size = data_size;
	one->next = NULL;

	if(_size == 0){
		_first_node = one;
		_last_node = one;
		_size++;
	}else{
		_last_node->next = one;
		_last_node = one;
		_size++;
	}

	return 1;
}

uint8_t Config::add(uint8_t kind, const char* data, uint8_t data_size){
	return add(kind, (char*)data, data_size+1);
}

// remove data from structure
uint8_t Config::remove(uint8_t kind){
	if(_size > 0){
		node_t* last = _first_node;
		node_t* one = _first_node;
		if(_first_node->kind == kind){
			_first_node = one->next;
			free(one->data);
			free(one);
			_size--;
			if(_size == 0){
				_first_node = NULL;
				_last_node = NULL;
			}
			return 1;
		}else{
			while(one != NULL){
				last = one;
				one = last->next;
				if(one->kind == kind){
					last->next = one->next;
					if(one == _last_node){
						_last_node = last;
					}
					free(one->data);
					free(one);
					_size--;
					return 1;
				}
			}
			return 0;
		}
	}
	return 0;
}

void Config::flush(){
	while(_size != 0){
		remove(_first_node->kind);
	}
}

uint8_t Config::getSize(){
	return _size;
}

// remove data from structure
void Config::print(){
	if(_size > 0){
		node_t* one = _first_node;
		while(one != NULL){
			Serial.print(one->kind);
			Serial.print(" - ");
			for(uint8_t i = 0 ; i < one->size ; i++){
				Serial.print(one->data[i]);
			}
			Serial.println("");
			one = one->next;
		}
	}
}

// get data from structure
char* Config::get(uint8_t kind){
	if(_size > 0){
		node_t* one = _first_node;
		while(one != NULL){
			if(one->kind == kind){
				return one->data;
			}
			one = one->next;
		}
	}
	return NULL;
}


/******************************************************************************

					STATIC FUNCTONS FOR DECODE IP FROM TEXT

******************************************************************************/


bool Config::isNumber(const String& str){
    return !str.empty() && (str.find_first_not_of("[0123456789]") == std::String::npos);
}


vector<String> Config::split(const String& str, char delim){
    auto i = 0;
    vector<String> list;

    auto pos = str.find(delim);

    while (pos != String::npos) {
        list.push_back(str.substr(i, pos - i));
        i = ++pos;
        pos = str.find(delim, pos);
    }

    list.push_back(str.substr(i, str.length()));

    return list;
}


bool Config::validateIP(String ip){

	vector<String> list = split(ip, '.');

	if(list.size() != 4){
		return false;
	}

	for(String str : list){
		if (!isNumber(str) || stoi(str) > 255 || stoi(str) < 0)
		return false;
	}

	return true;
}

/******************************************************************************

				PUBLIC FUNCTIONS FOR ADD DATA INTO STRUCTURE

******************************************************************************/

// AP mode functions

bool Config::_setByINDEX(uint8_t index, String& str){
	(void)remove(index);
	return (bool)add(index, str.c_str(), (uint8_t)strlen(str.c_str()));
}

String Config::_getByINDEX(uint8_t index){
	String ret = "";
	char* one = get(index);
	if(one != NULL){
		ret = String(one);
	}
	return ret;
}

bool Config::AP_setSSID(String& str){
	return _setByINDEX(I_AP_SSID, str);
}

bool Config::AP_setPASS(String& str){
	return _setByINDEX(I_AP_PASS, str);
}

bool Config::AP_setIP(String& str){
	return _setByINDEX(I_AP_IP, str);
}

bool Config::AP_setMASK(String& str){
	return _setByINDEX(I_AP_MASK, str);
}

bool Config::AP_setGATEWAY(String& str){
	return _setByINDEX(I_AP_GATEWAY, str);
}


String Config::AP_getSSID(){
	return _getByINDEX(I_AP_SSID);
}

String Config::AP_getPASS(){
	return _getByINDEX(I_AP_PASS);
}

String Config::AP_getIP(){
	return _getByINDEX(I_AP_IP);
}

String Config::AP_getMASK(){
	return _getByINDEX(I_AP_MASK);
}

String Config::AP_getGATEWAY(){
	return _getByINDEX(I_AP_GATEWAY);
}

// STATION MODE

bool Config::STATION_addConnection(String& ssid, String& pass){
	char* one = get(I_STATION_LIST);
	uint8_t pos = 255;
	if(one != NULL){
		uint8_t pos = atoi(one);
	}
	pos--;
	bool addedNoteSSID = (bool)add(pos, ssid.c_str(), (uint8_t)strlen(ssid.c_str()));
	pos--;
	bool addedNotePass = (bool)add(pos, pass.c_str(), (uint8_t)strlen(pass.c_str()));
	if(addedNoteSSID && addedNotePass){
		const char* strVal = String((char*)&pos).c_str();
		return add(I_STATION_LIST, strVal, (uint8_t)strlen(strVal));
	}
	return false;
	String((char*)str)

}

vector<pair<String,String>> Config::STATION_getConnection(){
	vector<pair<String,String>> ret;
	char* one = get(I_STATION_LIST);
	uint8_t pos = 255;
	if(one != NULL){
		uint8_t pos = atoi(one);
	}
	

}

uint8_t Config::STATION_getNumOfConnection(){

}
