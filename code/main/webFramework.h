#ifndef webFramework_h
#define webFramework_h

#include <Arduino.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <FS.h>
#include "ProjectConfig.h"
#include "Config.h"
#include "Controller.h"


#define WWW_SERVER_PORT 80
#define WF(FOO) std::bind(&webFramework::FOO, this, std::placeholders::_1)



class webFramework {


public:

	webFramework(Config* object, Controller* cont);
	String processor(const String& var);

	void begin();
private:
	AsyncWebServer server;
	Config* config;
	Controller* control;
	//String ledState;
	//EEPROM_M24512 eeprom;

	void handleRequest_index(AsyncWebServerRequest *request);
	void handleRequest_config(AsyncWebServerRequest *request);
	void handleRequest_configSave(AsyncWebServerRequest *request);
	void handleRequest_TextSave(AsyncWebServerRequest *request);
	void handleRequest_TextRemove(AsyncWebServerRequest *request);
	void handleRequest_wifiSTATION_JSON(AsyncWebServerRequest *request);
	void handleRequest_save(AsyncWebServerRequest *request);
	void handleRequest_load(AsyncWebServerRequest *request);
	void handleRequest_TemperatureData(AsyncWebServerRequest *request);
	void handleRequest_LabelData(AsyncWebServerRequest *request);

	void handleRequest_setDATA(AsyncWebServerRequest *request);
	void handleRequest_getTemperatureData(AsyncWebServerRequest *request);
	void handleRequest_getLedData(AsyncWebServerRequest *request);

	void handleRequest_jsonWifiNets(AsyncWebServerRequest *request);
	void handleRequest_WifiNets(AsyncWebServerRequest *request);


	uint16_t char2uint8_t(char* str);
};


#endif
