#include "Config.h"

Config::Config(){}

void Config::begin(){
	eeprom.begin();

	pinMode(PIN_RELE, OUTPUT);
	pinMode(PIN_BUTTON, INPUT);

	// print info about device
	char buffer[40];
	sprintf(buffer, "%1.5f V", ((float)ESP.getVcc())/20000);
	ConsolePrint("CHIP info VCC", buffer);
	ConsolePrint("CHIP info Chip ID", String(ESP.getChipId()).c_str());
	ConsolePrint("CHIP info Flash Chip ID", String(ESP.getFlashChipId()).c_str());
	ConsolePrint("CHIP info Core version", ESP.getCoreVersion().c_str());
	ConsolePrint("CHIP info SDK version", ESP.getSdkVersion());
	ConsolePrint("CHIP info Boot version", String(ESP.getBootVersion()).c_str());
	ConsolePrint("CHIP info CPU frequency", (String(ESP.getCpuFreqMHz()) + " MHz").c_str());
	ConsolePrint("CHIP info FREE HEAP", (String(ESP.getFreeHeap()) + " B").c_str());
	ConsolePrint("CHIP info FREE CONST. STACK", (String(ESP.getFreeContStack()) + " B").c_str());
	ConsolePrint("CHIP info real FLASH size", (String(ESP.getFlashChipRealSize()) + " B").c_str());


	/***************************************************************************/

	setNAME("esp8266");
	setSSID("ESP8266");
	setPASS("rootroot");
	setIP("192.168.0.1");
	setMASK("255.255.255.0");
	setGATEWAY("192.168.0.1");


	STATION_addConnection("PJTKV", "abeceda1");
	STATION_addConnection("UPC_2G", "Ab9876543210");


	/***************************************************************************/
}



/******************************************************************************

	FUNCTIONS FOR Static Variable ADD and GET DATA INTO LList STRUCTURE

******************************************************************************/


// return nondefine string if string is null
char* Config::_not2stringIfExist(char* one){
	if(one != NULL){
		return one;
	}
	return "Notdefine";
}


uint8_t Config::setNAME(char* str){
	if(strlen(str) <= SIZE_NAMEDEVICE){
		memcpy(name, str, strlen(str)+1);
		return 1;
	}
	return 0;
}

uint8_t Config::setSSID(char* str){
	if(strlen(str) <= SIZE_SSID){
		memcpy(ssid, str, strlen(str)+1);
		return 1;
	}
	return 0;
}

uint8_t Config::setPASS(char* str){
	if(strlen(str) <= SIZE_PASSWORD){
		memcpy(password, str, strlen(str)+1);
		return 1;
	}
	return 0;
}

uint8_t Config::setIP(char* str){
	uint8_t *one = StringToIP(str);
	uint8_t ret = 0;
	if(one[4] == 1){
		memcpy(ip, one, 4);
		ret = 1;
	}
	free(one);
	return ret;
}

uint8_t Config::setMASK(char* str){
	uint8_t *one = StringToIP(str);
	uint8_t ret = 0;
	if(one[4] == 1){
		memcpy(mask, one, 4);
		ret = 1;
	}
	free(one);
	return ret;
}

uint8_t Config::setGATEWAY(char* str){
	uint8_t *one = StringToIP(str);
	uint8_t ret = 0;
	if(one[4] == 1){
		memcpy(gateway, one, 4);
		ret = 1;
	}
	free(one);
	return ret;
}

void Config::_setIP(uint8_t* one){
	memcpy(ip, one, 4);
}

void Config::_setMASK(uint8_t* one){
	memcpy(mask, one, 4);
}

void Config::_setGATEWAY(uint8_t* one){
	memcpy(gateway, one, 4);
}


char* Config::getNAME(){
	return name;
}

char* Config::getSSID(){
	return ssid;
}

char* Config::getPASS(){
	return password;
}

char* Config::getIP(){
	return IPToString(ip);
}

char* Config::getMASK(){
	return IPToString(mask);
}

char* Config::getGATEWAY(){
	return IPToString(gateway);
}

uint8_t* Config::_getIP(){
	return ip;
}

uint8_t* Config::_getMASK(){
	return mask;
}

uint8_t* Config::_getGATEWAY(){
	return gateway;
}


char* Config::AP_Bprint(){
	Buffer buffer;
	buffer.printf("NAME: %s \n", getNAME());
	buffer.printf("SSID: %s \n", getSSID());
	buffer.printf("PASS: %s \n", getPASS());
	buffer.printf("  IP: %s \n", getIP());
	buffer.printf("MASK: %s \n", getMASK());
	buffer.printf("GATE: %s \n", getGATEWAY());
	return buffer.get();
}

/******************************************************************************

							STATION MODE

******************************************************************************/

// add data into structure
uint8_t Config::STATION_addConnection(char* ssid, char* pass){
	stationValues.removeByValue1(ssid);
	uint8_t max = stationValues.getMaxIndex();
	if(max < MAX_INDEX_LENG){
		return stationValues.add(max+1, ssid, pass);
	}
	return 0;
}

// return Buffer CONSOLE format data of seted WIFI connections
char* Config::STATION_getConnectionCONSLE(){
	Buffer buffer;
	buffer.printf("SETTED WIFI FOR CONNECTION\n");
	for(uint8_t i = 0 ; i <= stationValues.getMaxIndex() ; i++){
		note_t* one = stationValues.get_note(i);
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
	for(uint8_t i = 0 ; i <= stationValues.getMaxIndex() ; i++){
		note_t* one = stationValues.get_note(i);
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
void Config::STATION_removeConnection(uint8_t index){
	stationValues.remove(index);
}

void Config::STATION_removeConnectionByName(char* str){
	for(uint8_t i = 0 ; i <= stationValues.getMaxIndex() ; i++){
		note_t* one = stationValues.get_note(i);
		if(one != NULL){
			if(strcmp(one->value1, str) == 0){
				stationValues.remove(i);
			}
		}
	}
}

uint8_t Config::STATION_isInList(char* str){
	for(uint8_t i = 0 ; i <= stationValues.getMaxIndex() ; i++){
		note_t* one = stationValues.get_note(i);
		if(one != NULL){
			if(strcmp(one->value1, str) == 0){
				return 1;
			}
		}
	}
	return 0;
}


// return number of connections
uint8_t Config::STATION_getNumOfConnection(){
	return stationValues.getSize();
}

char* Config::STATION_Bprint(){
	return stationValues.Bprint_list();
}


/******************************************************************************

						SAVE LOAD FUNCTIONS EEPROM_M24512

******************************************************************************/

// sava structures into eeprom
void Config::save(){

	// PAGE 1
	uint8_t offset = 0;
	eeprom.write(PAGE_1, &offset, getNAME());
	eeprom.write(PAGE_1, &offset, getSSID());
	eeprom.write(PAGE_1, &offset, getPASS());
	eeprom.write_ip(PAGE_1, &offset, _getIP());
	eeprom.write_ip(PAGE_1, &offset, _getMASK());
	eeprom.write_ip(PAGE_1, &offset, _getGATEWAY());


	// PAGE 2
	offset = 0;
	eeprom._eeprom_write(PAGE_2, offset, STATION_getNumOfConnection());
	offset++;
	for(uint8_t i = 0 ; i <= stationValues.getMaxIndex() ; i++){
		note_t* one = stationValues.get_note(i);
		if(one != NULL){
			eeprom.write(PAGE_2, &offset, one->value1);
			eeprom.write(PAGE_2, &offset, one->value2);
		}
	}

	eeprom.printPageMemory(PAGE_2);

}

// load structures into eeprom
void Config::load(){

	// PAGE 1
	uint8_t offset = 0;
	setNAME(eeprom.read(PAGE_1, &offset));
	setSSID(eeprom.read(PAGE_1, &offset));
	setPASS(eeprom.read(PAGE_1, &offset));
	_setIP(eeprom.read_ip(PAGE_1, &offset));
	_setMASK(eeprom.read_ip(PAGE_1, &offset));
	_setGATEWAY(eeprom.read_ip(PAGE_1, &offset));


	// PAGE 2
	offset = 0;
	uint8_t size = eeprom._eeprom_read(PAGE_2, offset);
	offset++;
	for(uint8_t i = 0 ; i < size ; i++){
		char* name = eeprom.read(PAGE_2, &offset);
		char* pass = eeprom.read(PAGE_2, &offset);
		STATION_addConnection(name, pass);
	}


	Serial.println(STATION_getConnectionCONSLE());


}


void Config::Print(){
	Serial.println(STATION_Bprint());
}


/******************************************************************************

					PRIVATE FUNCTIONS FOR DECODE IP FROM TEXT

******************************************************************************/

// valide and decode string IP to array of uint8_t
// retunr 5 x uint8_t
// {(IP_1_byte),(IP_2_byte),(IP_3_byte),(IP_4_byte),(valide 1 or 0 for invalide)}
uint8_t* Config::StringToIP(char* str){
	uint8_t* ret = (uint8_t*)malloc(sizeof(uint8_t)*5);
	uint8_t len = strlen(str);
	uint16_t value = 0;
	uint8_t pos = 0;
	ret[4] = 1;
	if((len > 6) && (len < 16)){
		for(uint8_t i = 0 ; (i <= len) && (pos < 4) ; i++){
			char one = str[i];
			if((one == '.') || (one == '\0')){
				if(value < 256){
					ret[pos] = value;
					pos++;
					value = 0;
				}else{
					ret[4] *= 0;
				}
			}else if((one >= '0')&&(one <= '9')){
				value *= 10;
				value += one - 48;
			}else{
				ret[4] *= 0;
			}
		}
		if(pos != 4){
			ret[4] *= 0;
		}
	}else{
		ret[4] *= 0;
	}
	return ret;
}


char* Config::IPToString(uint8_t* input){
	String ret = "";
	ret += String(input[0]) + ".";
	ret += String(input[1]) + ".";
	ret += String(input[2]) + ".";
	ret += String(input[3]) + "\0";

	char* str = (char*)ret.c_str();
	memcpy(output, str, strlen(str)+1);
	return output;
}
