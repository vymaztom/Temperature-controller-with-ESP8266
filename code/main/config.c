#include <Wire.h>
#include <stdint.h>
//#include "ee"

#define MAXSIZEOFNAMEDEVICE 32
#define MAXSIZESSID 32
#define MAXSIZEWPA2PASSWORD 32

typedef struct config{
	char[MAXSIZEOFNAMEDEVICE] name;
	char[MAXSIZESSID] ssid;
	char[MAXSIZEWPA2PASSWORD] password;
	uint8_t[4] ip;
	uint8_t[4] mask;
	uint8_t[4] gateway;
}config_t;
