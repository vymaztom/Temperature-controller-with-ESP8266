#include <FS.h>
#include <LittleFS.h>
#include "WifiConfigJson.h"


#define DEBUG 1
#define DEBUG_PRINT1(note) do {if(DEBUG)Serial.printf("%s", note);} while (0);
#define DEBUG_PRINT2(note, note2) do {if(DEBUG)Serial.printf("%s %s", note, note2);} while (0);
#define DEBUG_PRINT3(note, note2, note3) do {if(DEBUG)Serial.printf("%s %s %s", note, note2, note3);} while (0);
#define DEBUG_PRINTLN1(note) do {if(DEBUG)Serial.printf("%s\n", note);} while (0);
#define DEBUG_PRINTLN2(note, note2) do {if(DEBUG)Serial.printf("%s %s\n", note, note2);} while (0);
#define DEBUG_PRINTLN3(note, note2, note3) do {if(DEBUG)Serial.printf("%s %s %s\n", note, note2, note3);} while (0);



WifiConfigJson::WifiConfigJson(){
	ReadConfig();
	//readFile("/config.json");
}


/*******************************************************************************

					Functions for work with filess

*******************************************************************************/

void WifiConfigJson::listDir(const char * dirname) {
	Serial.printf("Listing directory: %s\n", dirname);
	Dir dir = SPIFFS.openDir(dirname);
	while (dir.next()) {
		Serial.print(dir.fileName());
		File f = dir.openFile("r");
		Serial.println(f.size());
	}
}

void WifiConfigJson::readFile(const char * path) {
	File file = SPIFFS.open(path, "r");

	if(!file){
		Serial.printf("Error opening file %s for writing\n", path);
		return;
	}

	while(file.available()){
		Serial.write(file.read());
	}

	file.close();
}

void WifiConfigJson::writeFile(const char * path, const char * message) {
	Serial.printf("Creating file: %s\n", path);

	File file = SPIFFS.open(path, "wa");
	if (!file) {
		Serial.println("Failed to open file for writing");
		return;
	}
	if (file.print(message)) {
		Serial.println("OK");
	} else {
		Serial.println("FAIL");
	}
	delay(2000);
	file.close();
}

void WifiConfigJson::renameFile(const char * path1, const char * path2) {
	Serial.printf("Renaming file %s to %s\n", path1, path2);
	if (SPIFFS.rename(path1, path2)) {
		Serial.println("OK");
	} else {
		Serial.println("FAIL");
	}
}

void WifiConfigJson::deleteFile(const char * path) {
	Serial.printf("Deleting file: %s\n", path);
	if (SPIFFS.remove(path)) {
		Serial.println("OK");
	} else {
		Serial.println("FAIL");
	}
}

/*******************************************************************************

					My lib for config wifi from file

*******************************************************************************/

void WifiConfigJson::ReadConfig(){
	File file = SPIFFS.open("/config.json", "r");

	if (!file) {
		Serial.println("Error opening file for writing");
		return;
	}
	Serial.println();
	Serial.println("Reading config file");
	while (file.available()) {
		Serial.write(file.read());
	}
	//int bytesWritten = file.print("TEST SPIFFS");

	/*
	if (bytesWritten == 0) {
		Serial.println(file.read());
		return;
	}
	*/
	file.close();
}
