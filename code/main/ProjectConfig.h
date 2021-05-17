#ifndef ProjectConfig_h
#define ProjectConfig_h

#define DEBUG 1

#define DEBUG_SECTION(FUNCTION) if(DEBUG)Serial.printf(FUNCTION)

#define PRINT_SIZE 70

extern uint8_t run;
extern uint8_t language;
extern uint8_t active;

extern uint8_t status_red;
extern uint8_t status_yellow;
extern uint8_t status_green;

// PIN DEFINITIONS
#define PIN_DATA 12
#define PIN_RELE 14
#define PIN_BUTTON 16
#define PIN_SCL 5
#define PIN_SDA 4
#define PIN_A0 A0

#define PIN_LED_GREEN 13
#define PIN_LED_YELLOW 15

void ConsolePrint(char* text, char* status);
void ConsolePrint(char* text, const char* status);
void ConsolePrintMarkLine();

void led_green(uint8_t value);
void led_yellow(uint8_t value);
void LED_GREEN_TOGGLE();
#endif
