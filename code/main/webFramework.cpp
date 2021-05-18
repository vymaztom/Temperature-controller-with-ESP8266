#include "webFramework.h"
#include "WebTextVariables.h"
#include "ProjectConfig.h"


webFramework::webFramework(Config* object, Controller* cont):server(WWW_SERVER_PORT){
	config = object;
	control = cont;
	//eeprom.begin();
}


// Replaces placeholder with LED state value
String webFramework::processor(const String& var){
	char* input = (char*)var.c_str();
	String ret = String();
	if(input[0] == 't'){
		uint8_t value = char2uint8_t(input+1);
		char* str = WebTextVariable[language][value];
		//char* str = config->textValues_get(value, language);
		ret = String(str);
	}
	if(input[0] == 's'){
		if(input[1] == '0'){
			ret = String(config->getNAME());
		}else if(input[1] == '1'){
			ret = String(config->getIP());
		}else if(input[1] == '2'){
			ret = String(config->getMASK());
		}else if(input[1] == '3'){
			ret = String(config->getGATEWAY());
		}else if(input[1] == '4'){
			ret = String(config->getSSID());
		}else if(input[1] == '5'){
			ret = String(config->getPASS());
		}
	}
	return ret;
}




void webFramework::begin(){

	if(!SPIFFS.begin()){
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}

	/***************************** LINKS INTO FILES **************************/

	// Route to load style.css file
	server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/style.css", "text/css");
	});

	// Route to load javaScript.js file
	server.on("/javaScript.js", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/javaScript.js", "text/javascript");
	});


	// Route to load javaScript.js file
	server.on("/cross.png", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/cross.png", "image/png");
	});

	/****************************** WEB PAGES ********************************/

	// Route for root / web page
	server.on("/", HTTP_GET, WF(handleRequest_index));
	server.on("/index", HTTP_GET, WF(handleRequest_index));
	server.on("/index.html", HTTP_GET, WF(handleRequest_index));

	// Route to config page
	server.on("/config", HTTP_GET, WF(handleRequest_config));
	server.on("/config.html", HTTP_GET, WF(handleRequest_config));


	/****************************** ACTIONS **********************************/


	// Route to set GPIO to HIGH
	//server.on("/on", HTTP_GET, handleRequest_index);
	server.on("/on", HTTP_GET, WF(handleRequest_index));

	// Route to set GPIO to LOW
	//server.on("/off", HTTP_GET, handleRequest_index);
	server.on("/off", HTTP_GET, WF(handleRequest_index));

	// Route to config page
	server.on("/config/save", HTTP_ANY, WF(handleRequest_configSave));

	server.on("/config/text/save", HTTP_ANY, WF(handleRequest_TextSave));

	server.on("/config/text/remove", HTTP_ANY, WF(handleRequest_TextRemove));

	server.on("/setDATA", HTTP_ANY, WF(handleRequest_setDATA));

	server.on("/getTemperatureData", HTTP_ANY, WF(handleRequest_getTemperatureData));

	server.on("/getLedData", HTTP_ANY, WF(handleRequest_getLedData));

	server.on("/save", HTTP_ANY, WF(handleRequest_save));
	server.on("/load", HTTP_ANY, WF(handleRequest_load));



	server.on("/jsonWifiNets", HTTP_ANY, WF(handleRequest_jsonWifiNets));
	server.on("/WifiNets", HTTP_ANY, WF(handleRequest_WifiNets));

	/************************* PAGES FOR GET JSON ****************************/


	server.on("/wifiSTATION_JSON", HTTP_POST, WF(handleRequest_wifiSTATION_JSON));

	/*
	server.on("/TemperatureData", HTTP_ANY, WF(handleRequest_TemperatureData));

	server.on("/TemperatureLabel", HTTP_ANY, WF(handleRequest_LabelData));
	*/


	// Start server
	server.begin();
}


/*******************************************************************************

							HANDLERE FUNCTIONS

*******************************************************************************/




void webFramework::handleRequest_index(AsyncWebServerRequest *request){
	request->send(SPIFFS, "/index.html", String(), false, WF(processor));
}


void webFramework::handleRequest_config(AsyncWebServerRequest *request){
	request->send(SPIFFS, "/config.html", String(), false, WF(processor));
}


//http://192.168.0.227/config?a=1
void webFramework::handleRequest_configSave(AsyncWebServerRequest *request){
	int params = request->params();
	for(int i=0;i<params;i++){
		AsyncWebParameter* p = request->getParam(i);
		if(p->isPost()){
			if(strcmp(p->name().c_str(), "name") == 0){
				config->setNAME((char*)p->value().c_str());
			}
			if(strcmp(p->name().c_str(), "ip") == 0){
				config->setIP((char*)p->value().c_str());
			}
			if(strcmp(p->name().c_str(), "mask") == 0){
				config->setMASK((char*)p->value().c_str());
			}
			if(strcmp(p->name().c_str(), "gate") == 0){
				config->setGATEWAY((char*)p->value().c_str());
			}
			if(strcmp(p->name().c_str(), "ssid") == 0){
				config->setSSID((char*)p->value().c_str());
			}
			if(strcmp(p->name().c_str(), "pass") == 0){
				config->setPASS((char*)p->value().c_str());
			}
		}
	}

	request->redirect("/config");
}


void webFramework::handleRequest_TextSave(AsyncWebServerRequest *request){
	int params = request->params();
	uint8_t num = 0;
	char* ret = NULL;
	char* ret2 = NULL;
	for(int i=0;i<params;i++){
		AsyncWebParameter* p = request->getParam(i);
		if(p->isPost()){
			Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
			if(p->name().c_str()[0] == 'n'){
				num = char2uint8_t((char*)p->value().c_str());
			}
			if(p->name().c_str()[0] == 't'){
				ret = (char*)p->value().c_str();
			}
			if(p->name().c_str()[0] == '2'){
				ret2 = (char*)p->value().c_str();
			}
		}
	}

	request->redirect("/config");
}

void webFramework::handleRequest_TextRemove(AsyncWebServerRequest *request){
	Serial.printf("Handle remove\n");
	int params = request->params();
	uint8_t index = 0;
	for(int i=0;i<params;i++){
		AsyncWebParameter* p = request->getParam(i);
		if(p->isPost()){
			Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
			if(p->name().c_str()[0] == 'i'){
				index = char2uint8_t((char*)p->value().c_str());
			}
		}
	}

	request->redirect("/config");
}

void webFramework::handleRequest_wifiSTATION_JSON(AsyncWebServerRequest *request){
	String json = String(config->STATION_getConnectionJSON());
	request->send(200, "text/json", json);
}


void webFramework::handleRequest_save(AsyncWebServerRequest *request){
	config->save();
	request->redirect("/config");
}

void webFramework::handleRequest_load(AsyncWebServerRequest *request){
	config->load();
	request->redirect("/config");
}
/*
void webFramework::handleRequest_TemperatureData(AsyncWebServerRequest *request){
	String json = String(control->getTemperatureJSON());
	request->send(200, "text/json", json);
}


void webFramework::handleRequest_LabelData(AsyncWebServerRequest *request){
	String json = String(control->getLabelsJSON());
	request->send(200, "text/json", json);
}
*/

/*

*/



void webFramework::handleRequest_setDATA(AsyncWebServerRequest *request){
	int params = request->params();
	uint8_t index = 0;
	for(int i=0;i<params;i++){
		AsyncWebParameter* p = request->getParam(i);
		if(!p->isPost() && !p->isFile()){
			if(strcmp(p->name().c_str(), "temperature") == 0){
				control->setSetedTemeperature(p->value().toFloat());
			}
			if(strcmp(p->name().c_str(), "hysteresis") == 0){
				control->setSetedHysterez(p->value().toFloat());
			}
			if(strcmp(p->name().c_str(), "active") == 0){
				if(strcmp(p->value().c_str(), "true") == 0){
					active = 1;
				}else{
					active = 0;
				}
			}
		}
	}
	request->redirect("/index");
}

void webFramework::handleRequest_getTemperatureData(AsyncWebServerRequest *request){
	String json = String(control->getSetedDataJSON());
	request->send(200, "text/json", json);
}

void webFramework::handleRequest_getLedData(AsyncWebServerRequest *request){
	String json = "[{";
	if(status_green){
		json += "\"color1\":\"#5cb85c\",";
	}else{
		json += "\"color1\":\"white\",";
	}
	if(status_yellow){
		json += "\"color2\":\"orange\",";
	}else{
		json += "\"color2\":\"white\",";
	}
	if(status_red){
		json += "\"color3\":\"red\",";
	}else{
		json += "\"color3\":\"white\",";
	}
	json += "\"text1\":\""+String(WebTextVariable[language][17]) + "\",";
	json += "\"text2\":\""+String(WebTextVariable[language][18]) + "\",";
	json += "\"text3\":\""+String(WebTextVariable[language][19]) + "\"";

	json += "}]";
	request->send(200, "text/json", json);
}

void webFramework::handleRequest_jsonWifiNets(AsyncWebServerRequest *request){
	String json = "{";

	uint8_t Isfirst = true;
	uint8_t IsAddRSSI = false;

	json += "\"data\":[";
	for(uint8_t i = 0 ; i <= config->stationValues.getMaxIndex() ; i++){
		note_t* one = config->stationValues.get_note(i);
		if(one != NULL){
			if(Isfirst){
				Isfirst = false;
			}else{
				json += ",";
			}
			json += "{";
			json += "\"SSID\":\"" + String(one->value1) + "\",";
			IsAddRSSI = false;
			for(uint8_t j = 0; j < config->WifiNets_index; j++){
				if(strcmp(one->value1, config->WifiNets[j].SSID) == 0){
					json += "\"RSSI\":\"" + String(config->WifiNets[j].RSSI) + "\"";
					IsAddRSSI = true;
					break;
				}
			}
			if(!IsAddRSSI){
				json += "\"RSSI\":\"None\"";
			}
			json += "}";
		}
	}


	json += "]}";
	request->send(200, "text/json", json);
}

void webFramework::handleRequest_WifiNets(AsyncWebServerRequest *request){
	int params = request->params();
	uint8_t index = 0;

	char* name = NULL;
	char* pass = NULL;

	for(int i=0;i<params;i++){
		AsyncWebParameter* p = request->getParam(i);
		if(!p->isPost() && !p->isFile()){
			if(strcmp(p->name().c_str(), "remove") == 0){
				config->STATION_removeConnectionByName((char*)p->value().c_str());
			}
		}
		if(p->isPost()){
			if(strcmp(p->name().c_str(), "wifiAdd") == 0){
				name = (char*)p->value().c_str();
			}
			if(strcmp(p->name().c_str(), "passAdd") == 0){
				pass = (char*)p->value().c_str();
			}
		}
	}
	if((name != NULL)&&(pass != NULL)){
		config->STATION_addConnection(name, pass);
	}
	request->redirect("/config");
}


/*******************************************************************************

							TEXT DECODE FUNCTIONS

*******************************************************************************/

uint16_t webFramework::char2uint8_t(char* str){
	uint16_t ret = 0;
	for(uint8_t i = 0 ; i < strlen(str) ; i++){
		if((str[i] >= '0')&&(str[i] <= '9')){
			ret *= 10;
			ret += str[i] - 48;
		}else{
			ret = 256;
			return ret;
		}
	}
	return ret;
}
