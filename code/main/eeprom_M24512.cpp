#include "EEPROM_M24512.h"



/******************************************************************************

			PART OF FUNCTIONS FOR COMUNICATION WITH EEPROM M24512

*******************************************************************************/

EEPROM_M24512::EEPROM_M24512(){}


// begin function call I2C myWire begin
void EEPROM_M24512::begin(){
	myWire.setClock(M24512_CLOCK);
	myWire.begin(PIN_SDA, PIN_SCL);

}

// Print page from EEPROM MEMORY
void EEPROM_M24512::printPageMemory(uint16_t pageNumber){
	if(pageNumber < M24512_MAX_PAGES){
		uint16_t addr = pageNumber*M24512_PAGE_SIZE;
		myWire.beginTransmission(M24512_ADR);
		myWire.write(addr >> 8);
		myWire.write(addr & 0xFF);
		myWire.endTransmission();
		myWire.requestFrom(M24512_ADR, M24512_PAGE_SIZE);
		uint8_t position = 0;
		Serial.printf("PRINT EEPROM PAGE no. %u\n", pageNumber);
		while(myWire.available()){
			uint8_t one = myWire.read();
			Serial.printf("0x%02X,", one);
			if((one >= 32) && (one <= 126)){
				Serial.printf("(%3c)", one);
			}else{
				Serial.printf("(%3u)", one);
			}
			position++;
			if(position%M24512_PAGE_PRINT_LENGHT == 0){
				Serial.printf("\n");
			}
		}
	}
}


// read data by address from M24512
uint8_t EEPROM_M24512::M24512_read_byte(uint16_t addr){
	myWire.beginTransmission(M24512_ADR);
	myWire.write(addr >> 8);
	myWire.write(addr & 0xFF);
	myWire.endTransmission();
	myWire.requestFrom(M24512_ADR, 1);
	if(myWire.available() == 1){
		return myWire.read();
	}
	return 0xF;
}

// read data by address from M24512
char* EEPROM_M24512::M24512_read_byte(uint16_t addr, uint8_t size){
	myWire.beginTransmission(M24512_ADR);
	myWire.write(addr >> 8);
	myWire.write(addr & 0xFF);
	myWire.endTransmission();
	myWire.requestFrom((int)M24512_ADR, (int)size);
	char* ret = (char*) malloc(sizeof(char)*(size+1));
	uint8_t position = 0;
	while(myWire.available()){
		char c = myWire.read();
		ret[position] = c;
		position++;
	}
	ret[position] = '\0';
	return ret;
}

// write data at address into M24512
void EEPROM_M24512::M24512_write_byte(uint16_t addr, uint8_t data){
	myWire.beginTransmission(M24512_ADR);
	myWire.write(addr >> 8);
	myWire.write(addr & 0xFF);
	myWire.write(data);
	myWire.endTransmission();
	delay(10);
}

void EEPROM_M24512::M24512_write_byte(uint16_t addr, char* data){
	uint8_t len = strlen(data);
	Serial.printf(">>>>>>>>>>>>> WriteStringEEPROM %s LEN: %u\n", data, len);
	Serial.printf(">>>>>>>>>>>>>>>>> NEW addr: %u\n", addr);
	uint8_t pos = 0;
	while(pos < len){
		uint8_t diff = M24512_PAGE_SIZE - (addr%M24512_PAGE_SIZE);
		uint8_t miss = len-pos;
		Serial.printf(">>>>>>>>>>>>>>>>> DIFF: %u\n", diff);
		Serial.printf(">>>>>>>>>>>>>>>>> MISS: %u\n", miss);
		if(miss > diff){
			miss = diff;
		}
		myWire.beginTransmission(M24512_ADR);
		myWire.write(addr >> 8);
		myWire.write(addr & 0xFF);
		myWire.write((uint8_t*)(data+pos), miss);
		myWire.endTransmission();
		delay(10);
		pos += miss;
		addr += miss;
		Serial.printf(">>>>>>>>>>>>>>>>> NEW pos : %u\n", pos);
		Serial.printf(">>>>>>>>>>>>>>>>> NEW addr: %u\n", addr);
	}
}

// save data from structure into EEPROM
uint16_t EEPROM_M24512::M24512_saveLList(uint16_t offset, LList& list){
	uint16_t addr = offset;
	M24512_write_byte(addr, list.getSize());
	addr++;
	if(DEBUG){
		Serial.printf("Save> size: %u\n", list.getSize());
	}
	for(uint8_t i = 0 ; i <= list.getMaxIndex() ; i++){
		note_t* one = list.get_note(i);
		if(one != NULL){
			M24512_write_byte(addr, one->index);
			addr++;

			M24512_write_byte(addr, (uint8_t)strlen(one->value1));
			addr++;
			M24512_write_byte(addr, one->value1);
			addr += strlen(one->value1);

			M24512_write_byte(addr, (uint8_t)strlen(one->value2));
			addr++;
			M24512_write_byte(addr, one->value2);
			addr += strlen(one->value2);
			if(DEBUG){
				Serial.printf("Save> index: %u value1: %s (%u) value2: %s (%u)\n", one->index, one->value1, strlen(one->value1), one->value2, strlen(one->value2));
			}
		}
	}
	return addr;
}

// load data into structure from EEPROM
uint16_t EEPROM_M24512::M24512_loadLList(uint16_t offset, LList& list){
	list.flush();
	uint16_t addr = offset;
	uint8_t size = M24512_read_byte(addr);
	addr++;
	for(uint8_t i = 0 ; i < size ; i++){
		uint8_t index = M24512_read_byte(addr);
		addr++;

		uint8_t len1 = M24512_read_byte(addr);
		addr++;
		char* str1 = M24512_read_byte(addr, len1);
		addr += len1;

		uint8_t len2 = M24512_read_byte(addr);
		addr++;
		char* str2 = M24512_read_byte(addr, len2);
		addr += len2;
		if(DEBUG){
			Serial.printf("Load> index: %u value1: %s (%u) value2: %s (%u)\n", index, str1, len1, str2, len2);
		}
		list.add_malloced(index, str1, str2);
	}
	if(DEBUG){
		Serial.printf("offset> %u\n", addr);
	}
	return addr;
}
