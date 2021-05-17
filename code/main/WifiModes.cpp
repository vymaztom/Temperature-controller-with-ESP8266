
#include "WifiModes.h"


WifiModes::WifiModes(Config* conf){
	seted_mode = digitalRead(PIN_BUTTON);
}

void WifiModes::begin(){
	ConsolePrintMarkLine();

	pinMode(PIN_LED_GREEN, OUTPUT);
	pinMode(PIN_LED_YELLOW, OUTPUT);
	pinMode(PIN_RELE, OUTPUT);
	WiFi.disconnect();

	switch(seted_mode){
		case MODE_AP:
			toggle_LED = new Timer(1000, LED_GREEN_TOGGLE);
			ConsolePrint("WifiConfig select Mode", "AP (Server)");
			setAPmode();
			break;
		case MODE_STATION:
			ConsolePrint("WifiConfig select Mode", "STATION (Client)");
			setStationMode();
			break;
	}

	ConsolePrintMarkLine();
	// Start the mDNS responder for esp8266.local
	if(MDNS.begin("esp8266")){
		MDNS.addService("http", "tcp", 80);
		ConsolePrint("mDNS responder started", "OK");
	}else{
		ConsolePrint("mDNS responder started", "ERROR");
	}
}

void WifiModes::loop(){
	MDNS.update();
	MDNS.announce();
	if(digitalRead(PIN_BUTTON) != seted_mode){
		Serial.println("reset");
		ESP.reset();
	}
	if(seted_mode == MODE_AP){
		toggle_LED->run();
	}
}

void WifiModes::setAPmode(){
	const char *ssid_ = "ESP8266";
	const char *password_ = "rootroot";

	IPAddress Ip(192, 168, 2, 1);
	IPAddress NMask(255, 255, 255, 0);

	WiFi.mode(WIFI_AP);
  	WiFi.softAPConfig(Ip, Ip, NMask);


	WiFi.softAP(ssid_, password_ , findBestChanel(), 0, 4);

	ConsolePrint("WifiConfig AP MODE SSID", ssid_);
	char bufIP[40];
	IPAddress ip = WiFi.softAPIP();
	sprintf(bufIP, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	ConsolePrint("WifiConfig AP MODE IP", bufIP);

}

void WifiModes::setStationMode(){

	led_yellow(1);

	WiFi.mode(WIFI_STA);

	wifiMulti.addAP("PJTKV", "abeceda1");
	wifiMulti.addAP("UPC_2G", "Ab9876543210");


	ConsolePrint("WifiConfig Multi Wifi list connecting", "RUN");

	while(wifiMulti.run() != WL_CONNECTED){
		delay(1000);
	}
	led_yellow(0);
	led_green(1);

	ConsolePrint("WifiConfig STATION MODE SSID", WiFi.SSID().c_str());
	char bufIP[40];
	IPAddress ip = WiFi.localIP();
	sprintf(bufIP, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	ConsolePrint("WifiConfig STATION MODE IP", bufIP);
}

void WifiModes::wifiScan(){
	// Set WiFi to station mode and disconnect from an AP if it was previously connected
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	Serial.println("scan start");
	uint8_t chanels[14];
	for(uint8_t i = 0 ; i < 14 ; i++){
		chanels[i] = 0;
	}
	// WiFi.scanNetworks will return the number of networks found
	int n = WiFi.scanNetworks();
	Serial.println("scan done");
	if (n == 0) {
		Serial.println("no networks found");
	} else {
		Serial.print(n);
		Serial.println(" networks found");
		for (int i = 0; i < n; ++i) {
			// Print SSID and RSSI for each network found
			Serial.print(i + 1);
			Serial.print(": ");
			Serial.print(WiFi.SSID(i));
			Serial.print(" (");
			Serial.print(WiFi.RSSI(i));
			Serial.print(")");
			Serial.print((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " NOENCRYPTION " : " PASSED ");
			Serial.println(WiFi.channel(i));
			delay(10);
		}
	}
	Serial.println("");
}

int WifiModes::findBestChanel(){
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();

	uint8_t chanels[13];
	for(uint8_t i = 0 ; i < 13 ; i++){
		chanels[i] = 0;
	}

	uint8_t n = WiFi.scanNetworks();
	if (n == 0) {
		return 1;
	} else {
		for(uint8_t i = 0 ; i < n ; ++i){
			uint8_t ch = WiFi.channel(i)-1;
			if(ch < 14){
				uint8_t rssi_index = (uint8_t)((WiFi.RSSI(i)+100)/5);

				chanels[ch] += 16*rssi_index;

				if(ch > 0){
					chanels[ch-1] += 8*rssi_index;
				}
				if(ch > 1){
					chanels[ch-2] += 4*rssi_index;
				}
				if(ch > 2){
					chanels[ch-3] += 2*rssi_index;
				}
				if(ch > 3){
					chanels[ch-4] += 1*rssi_index;
				}

				if(ch < 12){
					chanels[ch+1] += 8*rssi_index;
				}
				if(ch < 11){
					chanels[ch+2] += 4*rssi_index;
				}
				if(ch < 10){
					chanels[ch+3] += 2*rssi_index;
				}
				if(ch < 9){
					chanels[ch+4] += 1*rssi_index;
				}
			}
		}
	}
	uint8_t min = 255;
	uint8_t min_pos = 0;
	for(uint8_t i = 0 ; i < 13 ; i++){
		if(min > chanels[i]){
			min = chanels[i];
			min_pos = i;
		}
	}
	ConsolePrint("WifiConfig select the best chanel", String(min_pos+1).c_str());
	/*
	for(uint8_t i = 0 ; i < 13 ; i++){
		Serial.print(i+1);
		Serial.print(" -> ");
		Serial.print(chanels[i]);
		Serial.print(" ");
		Serial.println("");
	}
	*/
	return (int)(min_pos+1);
}
