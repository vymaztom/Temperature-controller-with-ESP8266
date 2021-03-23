
#include "WifiModes.h"


WifiModes::WifiModes(Config* conf){
	seted_mode = digitalRead(PIN_BUTTON);
}

void WifiModes::begin(){
	switch(seted_mode){
		case MODE_AP:
			setAPmode();
			break;
		case MODE_STATION:
			setStationMode();
			break;
	}
}

void WifiModes::loop(){
	if(digitalRead(PIN_BUTTON) != seted_mode){
		Serial.println("reset");
		ESP.reset();
	}
}

void WifiModes::setAPmode(){
    Serial.println('\n');
	Serial.println('node AP\n');

	const char *ssid_ = "ESP8266";
	const char *password_ = "rootroot";

	IPAddress Ip(192, 168, 2, 1);
	IPAddress NMask(255, 255, 255, 0);

	WiFi.mode(WIFI_AP);
  	WiFi.softAPConfig(Ip, Ip, NMask);



	WiFi.softAP(ssid_, password_, 7, false, 8);



	Serial.print("Access Point \"");
    Serial.print(ssid_);
    Serial.println("\" started");

    Serial.print("IP address:\t");
    Serial.println(WiFi.softAPIP());

}


void WifiModes::setStationMode(){
	Serial.println('\n');
	Serial.println('node STATION\n');



	const char* ssid     = "UPC_2G";
	const char* password = "Ab9876543210";




	wifiMulti.addAP(ssid, password);
	wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
	wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");



	Serial.println("Connecting ...");
	while(wifiMulti.run() != WL_CONNECTED){
		delay(1000);
		Serial.print('.');
	}
	printWifiInfo();
}

void WifiModes::printWifiInfo(){
	Serial.println('\n');
	Serial.print("Connected to ");
	Serial.println(WiFi.SSID());
	Serial.print("IP address:\t");
	Serial.println(WiFi.localIP());
}
