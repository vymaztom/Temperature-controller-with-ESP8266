#include <Arduino.h>
#include <stdint.h>
#include "ProjectConfig.h"

uint8_t run = 1;
uint8_t language = 0;
uint8_t active = 0;

uint8_t status_red = 0;
uint8_t status_yellow = 0;
uint8_t status_green = 0;


void ConsolePrint(char* text, char* status){
	uint8_t size = strlen(text);
	Serial.print(text);
	for(uint8_t i = size ; i < PRINT_SIZE ; i++){
		Serial.print(".");
	}
	Serial.println(status);
}

void ConsolePrint(char* text, const char* status){
	uint8_t size = strlen(text);
	Serial.print(text);
	for(uint8_t i = size ; i < PRINT_SIZE ; i++){
		Serial.print(".");
	}
	Serial.println(status);
}

void ConsolePrintMarkLine(){
	for(uint8_t i = 0 ; i < PRINT_SIZE ; i++){
		Serial.print("-");
	}
	Serial.println("");
}

void LED_GREEN_TOGGLE(){
	led_green(!digitalRead(PIN_LED_GREEN));
}

void led_green(uint8_t value){
	status_green = value;
	digitalWrite(PIN_LED_GREEN, value);
}

void led_yellow(uint8_t value){
	status_yellow = value;
	digitalWrite(PIN_LED_YELLOW, value);
}
