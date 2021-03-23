
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <ESP8266WebServer.h>
#include <Wire.h>
#include <OneWire.h>
#include <DS18B20.h>
#include <ESP8266mDNS.h>





#include "webFramework.h"

#include "eeprom_M24512.h"
#include "Config.h"
#include "TelnetServer.h"
#include "WifiModes.h"

#ifndef APSSID
#define APSSID "ESPap"
#define APPSK  "heslo123"
#endif

Config conf;
webFramework webPage(&conf);
WifiModes WifiMode(&conf);
TelnetServer telnet;

/******************************************************************************

								MAIN CODE

******************************************************************************/




void setup() {
	delay(1000);
	Serial.begin(115200);
	conf.begin();
	telnet.begin();
	webPage.begin();
	WifiMode.begin();


	char* d_name = "name\0";
	char* d_ssid = "WIFI3\0";
	char* d_password = "passsssss\0";

	conf.add(I_NAME, d_name, 5);
	conf.add(v_ssid, d_ssid, 6);
	conf.add(v_password, d_password, 10);
	conf.print();

	//conf.remove(v_password);
	Serial.println("***************************");
	conf.print();

	conf.add(v_note, "note\0", 5);
	Serial.println("***************************");
	conf.print();

	//conf.save();
	conf.load();
	Serial.println("***************************");
	conf.print();






	/*
	Serial.println();
	Serial.print("Configuring access point...");
	*/


/******************************************************************************/

	/*
	WiFi.softAP(ssid, password);
	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	*/


	// Connect to Wi-Fi network with SSID and password
	/*
	Serial.print("Connecting to ");
	Serial.println(ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	*/

	/******************************************************************************/

/*
	server.on("/", handleRoot);
	server.begin();
	Serial.println("HTTP server started");
	*/
	/*
	Serial.println("");
	Serial.println("WiFi connected.");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	*/




	if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
		Serial.println("mDNS responder started");
	} else {
		Serial.println("Error setting up MDNS responder!");
	}

	//server.begin();

}



/******************************************************************************

									LOOP

******************************************************************************/



void loop(){
	MDNS.update();
	WifiMode.loop();
	//telnet.loop();
	delay(1000);
}
