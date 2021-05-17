#include "Controller.h"


Controller::Controller(Config* object):oneWire(PIN_DATA),sensors(&oneWire){
	startMillis = millis();
	config = object;
	_temperature_last_value = 0;
	_seted_temperature = 30.0;
	_seted_temperature_hysterez = 2.0;
	_status = 0;
}

void Controller::begin(){
	ConsolePrintMarkLine();
	// define pin modes
	pinMode(PIN_RELE, OUTPUT);

	// Start up the library
	sensors.begin();

	// load adrres of DallasTemperature
	if(sensors.getDeviceCount() > 0){
		ConsolePrint("DallasTemperature find first device", "OK");
		if(sensors.getAddress(Thermometer, 0)){
			ConsolePrint("DallasTemperature read address", "OK");
			sensors.setResolution(Thermometer, 9);
			led_yellow(0);
			run = 1;
		}else{
			ConsolePrint("DallasTemperature read address", "ERROR");
			led_yellow(1);
			run = 0;
		}
	}else{
		ConsolePrint("DallasTemperature find first device", "ERROR");
		led_yellow(1);
		run = 0;
	}

	_temperature_last_value = getTemperatureCelsius();
}

void Controller::loop(){
	unsigned long diffrentMillis = millis() - startMillis;
	if(diffrentMillis > TIMER_DIFFERENT){
		startMillis = millis();
		_temperature_last_value = getTemperatureCelsius();
		_control();
	}
}

float Controller::getTemperatureCelsius(){
	sensors.requestTemperatures();
	return sensors.getTempC(Thermometer);
}

void Controller::RELE_ON(){
	if(active){
		if(_status == 0){
			ConsolePrint("Controller RELE", "ON");
		}
		digitalWrite(PIN_RELE, 1);
		status_red = 1;
		_status = 1;

	}
}

void Controller::RELE_OFF(){
	if(_status == 1){
		ConsolePrint("Controller RELE", "OFF");
	}
	digitalWrite(PIN_RELE, 0);
	status_red = 0;
	_status = 0;
}

uint8_t Controller::RELE_STATUS(){
	return _status;
}

float Controller::getSetedTemeperature(){
	return _seted_temperature;
}

String Controller::getSetedDataJSON(){
	String json = "[{";
	if(_temperature_last_value != -127){
		json += "\"temperature\":" + String(_temperature_last_value) + ",";
	}else{
		json += "\"temperature\":\""+String(WebTextVariable[language][16]) + "\",";
	}
	json += "\"setedtemperature\":"+String(_seted_temperature) + ",";
	json += "\"hysterez\":"+String(_seted_temperature_hysterez) + ",";
	json += "\"active\":\"" + String(active) + "\",";
	json += "\"status\":"+String(_status);
	json += "}]";
	return json;
}

float Controller::getSetedHysterez(){
	return _seted_temperature_hysterez;
}

void Controller::setSetedTemeperature(float value){
	ConsolePrint("Controller seted temperature", String(value).c_str());
	_seted_temperature = value;
}

void Controller::setSetedHysterez(float value){
	ConsolePrint("Controller seted hysterez", String(value).c_str());
	_seted_temperature_hysterez = value;
}
/******************************************************************************

PRIVATE FUNCTIONS

******************************************************************************/

void Controller::_control(){
	if(!active){
		RELE_OFF();
	}
	if(_temperature_last_value != -127){
		if(_status == 1){
			if(_seted_temperature <= _temperature_last_value){
				RELE_OFF();
			}
		}else{
			if((_seted_temperature - _seted_temperature_hysterez) >=  _temperature_last_value){
				RELE_ON();
			}
		}
	}else{
		RELE_OFF();
		run = 0;
		led_yellow(1);
	}
}
