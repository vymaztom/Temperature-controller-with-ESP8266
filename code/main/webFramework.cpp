#include "webFramework.h"

webFramework::webFramework(Config* object):server(WWW_SERVER_PORT){
	config = object;
}


// Replaces placeholder with LED state value
String webFramework::processor(const String& var){
	if(var == "STATE"){
		String ledState;
		if(digitalRead(16)){
			ledState = "ON";
		}
		else{
			ledState = "OFF";
		}
		//Serial.print(ledState);
		return ledState;
	}
	return String();
}




void webFramework::begin(){

	if(!SPIFFS.begin()){
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}


	// Route to load style.css file
	server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/style.css", "text/css");
	});


	// Route for root / web page
	//server.on("/", HTTP_GET, handleRequest_index);
	server.on("/", HTTP_GET, WF(handleRequest_index));


	// Route to set GPIO to HIGH
	//server.on("/on", HTTP_GET, handleRequest_index);
	server.on("/on", HTTP_GET, WF(handleRequest_index));

	// Route to set GPIO to LOW
	//server.on("/off", HTTP_GET, handleRequest_index);
	server.on("/off", HTTP_GET, WF(handleRequest_index));


	//server.on("/config", HTTP_GET, handleRequest_config);
	server.on("/config", HTTP_GET, WF(handleRequest_config));

	server.on("/wifiConfig_JSON", HTTP_GET, WF(handleRequest_wifiConfig_JSON));


	// Start server
	server.begin();
}




void webFramework::handleRequest_index(AsyncWebServerRequest *request){
	request->send(SPIFFS, "/index.html", String(), false, WF(processor));
}


//http://192.168.0.227/config?a=1
void webFramework::handleRequest_config(AsyncWebServerRequest *request){
	int params = request->params();
	for(int i=0;i<params;i++){
		AsyncWebParameter* p = request->getParam(i);
		if(p->isFile()){ //p->isPost() is also true
			Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
		} else if(p->isPost()){
			Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
		} else {
			Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
		}
	}
	//request->send(SPIFFS, "/index.html", String(), false, WF(processor));
	request->redirect("/login");
}


void webFramework::handleRequest_wifiConfig_JSON(AsyncWebServerRequest *request){
	String json = "[";
	bool isFirtst = true;
	for(uint8_t i = 0 ; i < config->getSize() ; ++i){
		char* one = config->get((values)i);
		if(one != NULL){
			if(!isFirtst) json += ",";
			isFirtst = false;
			json += "{";
			Serial.printf("kind: %s, data:%s\n", String(i).c_str(), String(one).c_str());
			json += "\"kind\":"+String(i);
			json += ",\"data\":\""+String(one)+"\"";
			json += "}";
		}
	}
	json += "]";
	request->send(200, "text/json", json);
}
