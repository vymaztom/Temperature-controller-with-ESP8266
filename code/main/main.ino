
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <ESP8266WebServer.h>
#include <Wire.h>
#include <OneWire.h>
#include <ESP8266mDNS.h>





#include "webFramework.h"

#include "ProjectConfig.h"
#include "eeprom_M24512.h"
#include "Config.h"
#include "TelnetServer.h"
#include "WifiModes.h"
#include "LList.h"
#include "EEPROM_M24512.h"

#ifndef APSSID
#define APSSID "ESPap"
#define APPSK  "heslo123"
#endif


#include <OneWire.h>
#include <DallasTemperature.h>
#include <DS18B20.h>


OneWire oneWire(PIN_DATA);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);
DeviceAddress Thermometer;
int deviceCount = 0;

Config conf;
webFramework webPage(&conf);
WifiModes WifiMode(&conf);
TelnetServer telnet;

/******************************************************************************

								MAIN CODE

******************************************************************************/


LList one;


void setup() {
	delay(1000);
	Serial.begin(115200);
	conf.begin();
	telnet.begin();
	webPage.begin();
	WifiMode.begin();



	conf.GLOBAL_setNAME("esp8266");
	conf.AP_setSSID("ESP8266");
	conf.AP_setPASS("rootroot");
	conf.AP_setIP("192.168.1.1");
	conf.AP_setMASK("255.255.255.0");
	conf.AP_setGATEWAY("192.168.1.1");
	conf.AP_setNOTE("Tohle je Poznamka ktera je strasne dlouha a proto se nevleze do stranky Tohle je Poznamka ktera je strasne dlouha a proto se nevleze do stranky Tohle je Poznamka ktera je strasne dlouha a proto se nevleze do stranky Tohle je Poznamka ktera je strasne dlo1");
	Serial.print(conf.AP_Bprint());
	Serial.println("-------------------------------------");


	conf.STATION_addConnection("UPC_2G","Ab9876543210");
	conf.STATION_addConnection("MyNet14","123456789");
	conf.STATION_addConnection("NETNET","abeceda1");
	Serial.print(conf.STATION_getConnectionCONSLE());
	Serial.println("-------------------------------------");

	conf.save();
	Serial.println("saved");
	conf.load();
	Serial.println("load");



	Serial.println("-------------------------------------");
	Serial.print(conf.AP_Bprint());
	Serial.print(conf.STATION_getConnectionCONSLE());

	pinMode(PIN_RELE, OUTPUT);



	// Start up the library
    sensors.begin();

    // locate devices on the bus
    Serial.println("Locating devices...");
    Serial.print("Found ");
    deviceCount = sensors.getDeviceCount();
    Serial.print(deviceCount, DEC);
    Serial.println(" devices.");
    Serial.println("");

    Serial.println("Printing addresses...");
    for (int i = 0;  i < deviceCount;  i++)
    {
      Serial.print("Sensor ");
      Serial.print(i+1);
      Serial.print(" : ");
      sensors.getAddress(Thermometer, i);
      printAddress(Thermometer);
    }




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





	delay(1000);
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    Serial.print("0x");
    if (deviceAddress[i] < 0x10) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    if (i < 7) Serial.print(", ");
  }
  Serial.println("");
}
