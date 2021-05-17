#ifndef Controller_h
#define Controller_h

#include <Arduino.h>
#include <stdint.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "ProjectConfig.h"
#include "Config.h"
#include "Buffer.h"
#include "Timer.h"

extern char WebTextVariable[2][40][40];
#define TIMER_DIFFERENT 1000


class Controller {

public:
	Controller(Config* object);

	void begin();

	void loop();

	float getTemperatureCelsius();

	void RELE_ON();

	void RELE_OFF();

	uint8_t RELE_STATUS();

	float getSetedTemeperature();

	float getSetedHysterez();

	String getSetedDataJSON();

	void setSetedTemeperature(float value);

	void setSetedHysterez(float value);

private:

	Config* config;

	// DallasTemperature
	OneWire oneWire;
	DallasTemperature sensors;
	DeviceAddress Thermometer;

	// timers
	unsigned long startMillis;
	unsigned long counter;
	Timer* _temperature_read;

	float _temperature_last_value;
	float _seted_temperature;
	float _seted_temperature_hysterez;
	uint8_t _status;


	void _control();
};

#endif
