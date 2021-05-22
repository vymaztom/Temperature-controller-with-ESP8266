#include "EEPROM_24LC16B.h"



/******************************************************************************

			PART OF FUNCTIONS FOR COMUNICATION WITH EEPROM 24LC16B

*******************************************************************************/

EEPROM_24LC16B::EEPROM_24LC16B(){}


// begin function call I2C myWire begin
void EEPROM_24LC16B::begin(){
	myWire.begin();
}

// Print page from EEPROM MEMORY
void EEPROM_24LC16B::printPageMemory(uint8_t addr_i2c){
	uint8_t PAGE[256];

	Serial.printf("PRINT EEPROM PAGE no. 0x%X\n", addr_i2c);

	for(uint8_t i = 0 ; i < 255 ; i++){
		PAGE[i] = _eeprom_read(addr_i2c, i);
	}

	for(uint8_t i = 0 ; i < 255 ;){
		Serial.printf("0x%02X,", PAGE[i]);
		if((PAGE[i] >= 32) && (PAGE[i] <= 126)){
			Serial.printf("(%3c)", PAGE[i]);
		}else{
			Serial.printf("(%3u)", PAGE[i]);
		}
		i++;
		if(i%16 == 0){
			Serial.printf("\n");
		}
	}
	Serial.printf("\n");
}

void EEPROM_24LC16B::write(uint8_t addr_i2c, uint8_t* addr, char* data){
	uint16_t len = strlen(data);
	if((*addr + len + 1) <= 255){
		_eeprom_write(addr_i2c, *addr, (uint8_t)len);
		(*addr)++;
		for(uint8_t i = 0 ; i < len ; i++){
			_eeprom_write(addr_i2c, *addr, (uint8_t)data[i]);
			(*addr)++;
		}
	}
}

char* EEPROM_24LC16B::read(uint8_t addr_i2c, uint8_t* addr){
	uint8_t size = _eeprom_read(addr_i2c, *addr);
	(*addr)++;
	char* ret = (char*)malloc(sizeof(char)*(size+1));
	for(uint8_t i = 0 ; i < size ; i++){
		ret[i] = (char)_eeprom_read(addr_i2c, *addr);
		(*addr)++;
	}
	ret[size] = '\0';
	return ret;
}

void EEPROM_24LC16B::write_ip(uint8_t addr_i2c, uint8_t* addr, uint8_t* data){
	if((*addr + 4) <= 255){
		_eeprom_write(addr_i2c, *addr, data[0]);
		(*addr)++;
		_eeprom_write(addr_i2c, *addr, data[1]);
		(*addr)++;
		_eeprom_write(addr_i2c, *addr, data[2]);
		(*addr)++;
		_eeprom_write(addr_i2c, *addr, data[3]);
		(*addr)++;
	}
}

uint8_t* EEPROM_24LC16B::read_ip(uint8_t addr_i2c, uint8_t* addr){
	retIP[0] = _eeprom_read(addr_i2c, *addr);
	(*addr)++;
	retIP[1] = _eeprom_read(addr_i2c, *addr);
	(*addr)++;
	retIP[2] = _eeprom_read(addr_i2c, *addr);
	(*addr)++;
	retIP[3] = _eeprom_read(addr_i2c, *addr);
	(*addr)++;
	Serial.println("EEPROM " + String(retIP[0]) + "." + String(retIP[1]) + "." + String(retIP[2]) + "." + String(retIP[3]));
	return retIP;
}

/******************************************************************************

					PRIVATE FUNCTIONS FOR EEPROM 24LC16B

*******************************************************************************/

// Read data from eeprom by address and selected i2c address
uint8_t EEPROM_24LC16B::_eeprom_read(uint8_t chip_addr, uint8_t addr){
	uint8_t ret = 0;
	myWire.beginTransmission((int)chip_addr);
	myWire.write((byte)addr);
	myWire.endTransmission();
	myWire.requestFrom((int)chip_addr, 1);
	if(myWire.available() == 1){
		ret = (uint8_t)myWire.read();
	}else{
		delay(10);
		return _eeprom_read_rec(chip_addr, addr, 0, 5);
	}
	delay(10);
	return ret;
}

// Read data from eeprom by address and selected i2c address
uint8_t EEPROM_24LC16B::_eeprom_read_rec(uint8_t chip_addr, uint8_t addr, uint8_t deep, uint8_t maxdeep){
	if(deep < maxdeep){
		myWire.beginTransmission((int)chip_addr);
		myWire.write((byte)addr);
		myWire.endTransmission();
		myWire.requestFrom((int)chip_addr, 1);
		if(myWire.available() == 1){
			return (uint8_t)myWire.read();
		}else{
			delay(10);
			return _eeprom_read_rec(chip_addr, addr, deep+1, maxdeep);
		}
	}else{
		ConsolePrint("EEPROM  read", "ERROR");
		ConsolePrint("EEPROM  read", "NO DATA");
	}
	return 0;
}


// Write data by address into eeprom selected by address
void EEPROM_24LC16B::_eeprom_write(uint8_t chip_addr, uint8_t addr, uint8_t data){
	myWire.beginTransmission(chip_addr);
	myWire.write((byte)addr);
	myWire.write((byte)data);
	uint8_t i2c_state = myWire.endTransmission();
	delay(10);

	if(i2c_state != 0){
		/*
		switch(i2c_state){
			case 1:
				ConsolePrint("EEPROM ERROR", "Data to long");
				break;
			case 2:
				ConsolePrint("EEPROM ERROR", "Addr NACK");
				break;
			case 3:
				ConsolePrint("EEPROM ERROR", "Data NACK");
				break;
			case 4:
				ConsolePrint("EEPROM ERROR", "Other");
				break;
		}
		*/

		if(_eeprom_write_rec(chip_addr,addr,data,0, 5) != 1){
			ConsolePrint("EEPROM  rec. write", "FATALERROR (max deep)");
		}

	}

}

// Write data by address into eeprom selected by address
uint8_t EEPROM_24LC16B::_eeprom_write_rec(uint8_t chip_addr, uint8_t addr, uint8_t data, uint8_t deep, uint8_t maxdeep){
	if(maxdeep > deep){
		delay(10);
		myWire.beginTransmission(chip_addr);
		myWire.write((int)addr);
		myWire.write((int)data);
		if(myWire.endTransmission() != 0){
			//ConsolePrint("EEPROM  rewrite", "ERROR");
			return _eeprom_write_rec(chip_addr,addr,data,deep++, maxdeep);
		}else{
			return 1;
		}
	}
	return 0;
}
