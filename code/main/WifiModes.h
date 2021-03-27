#ifndef WifiModes_h
#define WifiModes_h

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "ProjectConfig.h"
#include "Config.h"

#define MODE_AP 1
#define MODE_STATION 0

class WifiModes {


public:

	WifiModes(Config* object);
	void begin();
	void loop();
private:
	uint8_t seted_mode;
	Config* config;
	ESP8266WiFiMulti wifiMulti;
	void setAPmode();
	void setStationMode();
	void printWifiInfo();


};


#endif
