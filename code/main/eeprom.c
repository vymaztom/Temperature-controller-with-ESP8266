#include <Wire.h>
#include <stdint.h>

// I2C address of the M24512
#define M24512_ADR 0x50

// write data at address into M24512
void M24512_write_byte(uint16_t addr, uint8_t data){
	Wire.beginTransmission(M24512_ADR);
	Wire.write(addr >> 8);
	Wire.write(addr & 0xFF);
	Wire.write(data);
	Wire.endTransmission();
}

// read data by address from M24512
uint8_t M24512_read_byte(uint16_t addr){
	Wire.beginsTransmission(M24512_ADR);
	Wire.write(addr >> 8);
	Wire.write(addr & 0xFF);
	Wire.endTransmission();
	Wire.requestFrom(Addr, 1);
	if(Wire.available() == 1){
		return Wire.read();
	}
	return 0xFF;
}
