
#include "WifiModes.h"


WifiModes::WifiModes(Config* conf){
	config = conf;
	seted_mode = digitalRead(PIN_BUTTON);
	config->setWifiMode(seted_mode);
}

void WifiModes::begin(){
	ConsolePrintMarkLine();
	wifiScan();


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
	if(MDNS.begin(config->getNAME())){
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
		ConsolePrint("ESP", "reset");
		ESP.reset();
	}
	if(seted_mode == MODE_AP){
		toggle_LED->run();
	}
}

void WifiModes::setAPmode(){

	uint8_t* ipadr = config->_getIP();
	uint8_t* mask = config->_getMASK();
	uint8_t* gateway = config->_getGATEWAY();

	IPAddress Ip(ipadr[0], ipadr[1], ipadr[2], ipadr[3]);
	IPAddress NMask(mask[0], mask[1], mask[2], mask[3]);
	IPAddress Gate(gateway[0], gateway[1], gateway[2], gateway[3]);

	WiFi.mode(WIFI_AP);
  	WiFi.softAPConfig(Ip, Gate, NMask);


	WiFi.softAP(config->getSSID(), config->getPASS(), findBestChanel(), 0, 4);

	ConsolePrint("WifiConfig AP MODE SSID", config->getSSID());
	char bufIP[40];
	IPAddress ip = WiFi.softAPIP();
	sprintf(bufIP, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	ConsolePrint("WifiConfig AP MODE IP", bufIP);

}

void WifiModes::setStationMode(){

	led_yellow(1);

	WiFi.mode(WIFI_STA);

	for(uint8_t i = 0 ; i <= config->stationValues.getMaxIndex() ; i++){
		note_t* one = config->stationValues.get_note(i);
		if(one != NULL){
			wifiMulti.addAP(one->value1, one->value2);
		}
	}

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

	config->dhcp[0] = ip[0];
	config->dhcp[1] = ip[1];
	config->dhcp[2] = ip[2];
	config->dhcp[3] = ip[3];
	config->Save_buttons();
}

void WifiModes::wifiScan(){

	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	config->WifiNets_index = 0;


	int n = WiFi.scanNetworks();

	for(int i = 0; i < n; ++i){
		memcpy(config->WifiNets[config->WifiNets_index].SSID, String(WiFi.SSID(i)).c_str(), strlen(String(WiFi.SSID(i)).c_str())+1);
		config->WifiNets[config->WifiNets_index].RSSI = WiFi.RSSI(i);
		switch(WiFi.encryptionType(i)){
			case 2:
				memcpy(config->WifiNets[config->WifiNets_index].SECURE, "TKIP (WPA)", 11);
				break;
			case 5:
				memcpy(config->WifiNets[config->WifiNets_index].SECURE, "WEP", 4);
				break;
			case 4:
				memcpy(config->WifiNets[config->WifiNets_index].SECURE, "CCMP (WPA)", 11);
				break;
			case 7:
				memcpy(config->WifiNets[config->WifiNets_index].SECURE, "NONE", 5);
				break;
			case 8:
				memcpy(config->WifiNets[config->WifiNets_index].SECURE, "AUTO", 5);
				break;
		}
		config->WifiNets_index++;
	}
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
