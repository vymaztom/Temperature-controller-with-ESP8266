#ifndef webFramework_h
#define webFramework_h

#include <Arduino.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <FS.h>
#include "Config.h"

#define WWW_SERVER_PORT 80
#define WF(FOO) std::bind(&webFramework::FOO, this, std::placeholders::_1)

class webFramework {


public:

	webFramework(Config* object);
	String processor(const String& var);

	void begin();
private:
	AsyncWebServer server;
	Config* config;
	//String ledState;
	void handleRequest_index(AsyncWebServerRequest *request);
	void handleRequest_config(AsyncWebServerRequest *request);
	void handleRequest_wifiConfig_JSON(AsyncWebServerRequest *request);
};


#endif
