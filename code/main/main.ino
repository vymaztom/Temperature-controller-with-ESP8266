
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <OneWire.h>


#include "webFramework.h"

#include "ProjectConfig.h"
#include "Config.h"
#include "WifiModes.h"
#include "LList.h"
#include "EEPROM_24LC16B.h"
#include "Controller.h"






Config conf;
Controller control(&conf);
webFramework webPage(&conf, &control);
WifiModes WifiMode(&conf);
//EEPROM_24LC16B eeprom;


/******************************************************************************

								MAIN CODE

******************************************************************************/






void setup(){
	delay(1000);

	Serial.begin(115200);

/*
	eeprom.begin();
	uint8_t offset = 0;
	uint8_t _offset = 0;
	eeprom.write(PAGE_1, &offset, "esp8266");
	Serial.printf("read: %s\n", eeprom.read(PAGE_1, &_offset));
	Serial.printf("%u _%u\n", offset, _offset);
	eeprom.printPageMemory(PAGE_1);
*/


	/*
	Wire.begin();
	eeprom_write(0x50, 0, 200);
	eeprom_write(0x51, 0, 201);
	eeprom_write(0x52, 0, 202);
	eeprom_write(0x53, 0, 203);
	eeprom_write(0x54, 0, 204);
	eeprom_write(0x55, 0, 205);
	eeprom_write(0x56, 0, 206);
	eeprom_write(0x57, 0, 207);

	Serial.printf("read data: %u\n", eeprom_read(0x50, 0));
	Serial.printf("read data: %u\n", eeprom_read(0x51, 0));
	Serial.printf("read data: %u\n", eeprom_read(0x52, 0));
	Serial.printf("read data: %u\n", eeprom_read(0x53, 0));
	Serial.printf("read data: %u\n", eeprom_read(0x54, 0));
	Serial.printf("read data: %u\n", eeprom_read(0x55, 0));
	Serial.printf("read data: %u\n", eeprom_read(0x56, 0));
	Serial.printf("read data: %u\n", eeprom_read(0x57, 0));
	*/

	conf.begin();
	webPage.begin();
	WifiMode.begin();
	control.begin();



/*
	digitalWrite(PIN_LED_GREEN, 1);
	delay(1000);
	digitalWrite(PIN_LED_YELLOW, 1);
	delay(1000);
	digitalWrite(PIN_RELE, 1);
*/
	/*
	conf.load();

	#define MOVE_PAGE 512

	conf.AP_setNOTE("NoteAAAAAA");
	Serial.printf("Read: %u\n", conf.eeprom.M24512_read_byte(MOVE_PAGE));
	conf.eeprom.M24512_write_byte(MOVE_PAGE,'Z');
	Serial.printf("Read: %u\n", conf.eeprom.M24512_read_byte(MOVE_PAGE));
	conf.eeprom.printPageMemory(conf.eeprom.M24512_getNumberOfPageByAddres(MOVE_PAGE));
/*
	conf.GLOBAL_setNAME("esp8266");
	conf.AP_setSSID("ESP8266");
	conf.AP_setPASS("rootroot");
	conf.AP_setIP("192.168.1.1");
	conf.AP_setMASK("255.255.255.0");
	conf.AP_setGATEWAY("192.168.1.1");
	Serial.print(conf.AP_Bprint());
	Serial.println("-------------------------------------");


	conf.STATION_addConnection("UPC_2G","Ab9876543210");
	conf.STATION_addConnection("MyNet14","123456789");
	conf.STATION_addConnection("NETNET","abeceda1");
	Serial.print(conf.STATION_getConnectionCONSLE());
	Serial.println("-------------------------------------");

	//conf.save();

	Serial.println("saved");

	Serial.println("load");
	//conf.load();

	conf.AP_setNOTE("Note 2");
	conf.textValues_add(1, "Regulátor teploty", "Temperature controller");

	//conf.save();
	//conf.load();


	Serial.println("-------------------------------------");

	conf.STATION_addConnection("WIFI","AAAAAAAAAAAA");
	conf.Print();

	/*
	Serial.print(conf.AP_Bprint());
	Serial.print(conf.STATION_getConnectionCONSLE());
	Serial.print(conf.textValues_Bprint());
	*/



}



/******************************************************************************

									LOOP

******************************************************************************/

void loop(){
	WifiMode.loop();
	if(run){
		control.loop();
	}else{
		control.begin();
	}
}
/*
uint8_t eeprom_read(uint8_t chip_addr, uint8_t addr){
	uint8_t ret = 0;
	Wire.beginTransmission(chip_addr);
	Wire.write(addr);
	if(Wire.endTransmission() != 0){
		ConsolePrint("EEPROM  read", "ERROR");
	}
	Wire.requestFrom(chip_addr, 1);
	if(Wire.available() == 1){
		ret = Wire.read();
	}else{
		ConsolePrint("EEPROM  read", "NO DATA");
	}
	delay(5);
	return ret;
}

void eeprom_write(uint8_t chip_addr, uint8_t addr, uint8_t data){
	Wire.beginTransmission(chip_addr);
	Wire.write(addr);
	Wire.write(data);
	if(Wire.endTransmission() != 0){
		ConsolePrint("EEPROM  write", "ERROR");
	}
	delay(5);
}
*/
