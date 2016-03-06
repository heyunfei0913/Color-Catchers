#ifndef ___custom_char_h__
#define __custom_char_h__

void LCDdefinechar(const uint8_t *pc,uint8_t char_code);

const char darkBall[8] PROGMEM = {
	0b00000,
	0b01110,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b01110,
	0b00000
};

const char lightBall[8] PROGMEM = {
	0b00000,
	0b01110,
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b01110,
	0b00000
};

const char halfBall[8] PROGMEM = {
	0b00000,
	0b01110,
	0b10001,
	0b10011,
	0b10111,
	0b11111,
	0b01110,
	0b00000
};

const char heart[8] PROGMEM = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000
};

const char trophyLeft[8] PROGMEM = {
	0b00111,
	0b01111,
	0b10110,
	0b01110,
	0b00011,
	0b00001,
	0b00001,
	0b00110
};

const char trophyRight[8] PROGMEM = {
   0b11100,
	0b11110,
	0b01101,
	0b01110,
	0b11000,
	0b10000,
	0b10000,
	0b01100
};

const char skull[8] PROGMEM = {
   0b01110,
	0b01110,
	0b10101,
	0b10101,
	0b01010,
	0b01010,
	0b01110,
	0b10101
};

const char leftBigWave[8] PROGMEM = {
	0b00001,
	0b01010,
	0b10010,
	0b10010,
	0b10010,
	0b10010,
	0b01010,
	0b00001
};

const char leftSmallWave[8] PROGMEM = {
	0b00000,
	0b10000,
	0b01010,
	0b01001,
	0b01001,
	0b01010,
	0b10000,
	0b00000
};

const char rightBigWave[8] PROGMEM = {
	0b10000,
	0b01010,
	0b01001,
	0b01001,
	0b01001,
	0b01001,
	0b01010,
	0b10000
};

const char rightSmallWave[8] PROGMEM = {
	0b00000,
	0b00001,
	0b01010,
	0b10010,
	0b10010,
	0b01010,
	0b00001,
	0b00000
};

#endif