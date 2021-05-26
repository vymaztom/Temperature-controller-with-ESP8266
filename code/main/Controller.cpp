#include "Controller.h"


Controller::Controller(Config* object):oneWire(PIN_DATA),sensors(&oneWire),client_logger(),dht(PIN_DATA, DHT22){
	startMillis = millis();
	startMillis_Logger = millis();
	config = object;
	_temperature_last_value = 0;
	_status = 0;
}

void Controller::begin(){
	//ConsolePrintMarkLine();
	// define pin modes
	pinMode(PIN_RELE, OUTPUT);

	// load seted data from EEPROM
	_seted_temperature = ((float)config->eeprom._eeprom_read(PAGE_4, 0))-20;
	_seted_temperature_hysterez = (float)config->eeprom._eeprom_read(PAGE_4, 1);
	ConsolePrint("Controller read from EEPROM Temperature", String(_seted_temperature).c_str());
	ConsolePrint("Controller read from EEPROM Hysteresis", String(_seted_temperature_hysterez).c_str());







	// Start up the library
	sensors.begin();

	ThingSpeak.begin(client_logger);

	// load adrres of DallasTemperature
	if(sensors.getDeviceCount() > 0){
		ConsolePrint("DallasTemperature find first device", "OK");
		if(sensors.getAddress(Thermometer, 0)){
			ConsolePrint("DallasTemperature read address", "OK");
			sensors.setResolution(Thermometer, 9);
			led_yellow(0);
			run = 1;
			config->selected_sensor = 0;
		}else{
			//ConsolePrint("DallasTemperature read address", "ERROR");
			led_yellow(1);
			run = 0;
		}
	}else{
		//ConsolePrint("DallasTemperature find first device", "ERROR");
		led_yellow(1);
		run = 0;
	}

	if(run == 0){
		dht.begin();
		if(!isnan(dht.readTemperature())){
			ConsolePrint("DHT 22 read temperature", "OK");
			led_yellow(0);
			run = 1;
			config->selected_sensor = 1;
		}
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



	unsigned long diffrentMillis_Logger = millis() - startMillis_Logger;
	if(diffrentMillis_Logger > config->getTimerLogger()){
		startMillis_Logger = millis();
		if(config->loggerOnOff){
			int x = ThingSpeak.writeField(config->getMyChanelNumber(), 1, _temperature_last_value, config->getApiKey());
			if(x == 200){
				ConsolePrint("IoT Cloud send", "OK");
			}else{
				ConsolePrint("IoT Cloud send", "ERROR");
			}
		}else{
			ConsolePrint("IoT Cloud send", "Deactivate");
		}
	}
}

float Controller::getTemperatureCelsius(){
	if(config->selected_sensor == 0){
		sensors.requestTemperatures();
		return sensors.getTempC(Thermometer);
	}else if(config->selected_sensor == 1){
		return dht.readTemperature();
	}
}

void Controller::RELE_ON(){
	if(config->controllerOnOff){
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
	if((_temperature_last_value != -127) && (!isnan(_temperature_last_value))){
		json += "\"temperature\":" + String(_temperature_last_value) + ",";
	}else{
		json += "\"temperature\":\""+String(WebTextVariable[language][16]) + "\",";
	}
	json += "\"setedtemperature\":"+String(_seted_temperature) + ",";
	json += "\"hysterez\":"+String(_seted_temperature_hysterez) + ",";
	json += "\"active\":\"" + String(config->controllerOnOff) + "\",";
	json += "\"status\":"+String(_status);
	json += "}]";
	return json;
}

float Controller::getSetedHysterez(){
	return _seted_temperature_hysterez;
}

void Controller::setSetedTemeperature(float value){
	ConsolePrint("Controller seted temperature", String(value).c_str());
	config->eeprom._eeprom_write(PAGE_4, 0, (uint8_t)(value+20));
	_seted_temperature = value;
	ConsolePrint("Controller degub", String((uint8_t)(value+20)).c_str());
}

void Controller::setSetedHysterez(float value){
	ConsolePrint("Controller seted hysterez", String(value).c_str());
	config->eeprom._eeprom_write(PAGE_4, 1, (uint8_t)value);
	_seted_temperature_hysterez = value;
	ConsolePrint("Controller degub", String((uint8_t)value).c_str());
}


/******************************************************************************

PRIVATE FUNCTIONS

******************************************************************************/

void Controller::_control(){
	if(!config->controllerOnOff){
		RELE_OFF();
	}
	if((_temperature_last_value != -127) && (!isnan(_temperature_last_value))){
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
		ConsolePrint("Temperature sensor", "Disconnected");
	}
}
