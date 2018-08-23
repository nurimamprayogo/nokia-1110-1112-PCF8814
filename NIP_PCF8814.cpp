#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif
#include <stdlib.h>
#include <avr/pgmspace.h>

#include "NIP_PCF8814.h"
#include <Adafruit_GFX.h>

static uint8_t lcd_memory[LCDWIDTH*((LCDHEIGHT/8)+1)];
static uint8_t xUpdateMin, xUpdateMax, yUpdateMin, yUpdateMax;

NIP_PCF8814::NIP_PCF8814(char SCLK, char DIN, char CS, char RST, char BL) : Adafruit_GFX(LCDWIDTH, LCDHEIGHT) {
	pin_SCLK=SCLK;
	pin_DIN=DIN;
	pin_RST=RST;
	pin_CS=CS;
	pin_BL=BL;
}

NIP_PCF8814::NIP_PCF8814(char SCLK, char DIN, char CS, char RST) : Adafruit_GFX(LCDWIDTH, LCDHEIGHT) {
	pin_SCLK=SCLK;
	pin_DIN=DIN;
	pin_RST=RST;
	pin_CS=CS;
	pin_BL=255;
}

void updateBoundingBox (int xmin, int ymin, int xmax, int ymax) {
	if (xmin < xUpdateMin) xUpdateMin = xmin;
	if (xmax > xUpdateMax) xUpdateMax = xmax;
	if (ymin < yUpdateMin) yUpdateMin = ymin;
	if (ymax > yUpdateMax) yUpdateMax = ymax;
}

void NIP_PCF8814::begin()
{
    pinMode(pin_SCLK,  OUTPUT);
    pinMode(pin_DIN,  OUTPUT);
    pinMode(pin_RST, OUTPUT);
    pinMode(pin_CS,   OUTPUT);
	pinMode(pin_BL,   OUTPUT);

	digitalWrite(pin_RST, HIGH);
	digitalWrite(pin_CS,   HIGH);
	digitalWrite(pin_SCLK,   LOW);
	analogWrite(pin_BL,   200);
	
	send(PCF8814_CMD, LCD_SOFT_RESET); 
	send(PCF8814_CMD, LCD_EXT_OSC); 
	send(PCF8814_CMD, LCD_FRAME_FREQ); 
	send(PCF8814_CMD, LCD_PARTIAL_DISPLAY); 
	send(PCF8814_CMD, LCD_CHARGE_PUMP_ON);
	
	setContrast(LCD_CONTRAST);
	rotate(false);
	lcd_on();
	delay(200);
	clearDisplay();
	setCursor(0, 0);
	int index;
	for(index=0; index < LCDWIDTH*((LCDHEIGHT/8)+1); index++)
		send(PCF8814_DATA, 0x00);
}

void NIP_PCF8814::clearDisplay() {
/*	
	setCursor(0, 0);
	int index;
	for(index=0; index < 864; index++)
		send(PCF8814_DATA, 0x00);
*/
	
	for (int index = 0; index < LCDWIDTH*((LCDHEIGHT/8)+1) ; index++)
  {
   lcd_memory[index] = (0x00);
  }
  updateBoundingBox(0, 0, LCDWIDTH - 1, LCDHEIGHT - 1); 
}

void NIP_PCF8814::clearLine()
{
    setCursor(0, line);

    for (unsigned char i = 0; i < LCDWIDTH; i++) {
        send(PCF8814_DATA, 0x00);
    }

    setCursor(0, line);
}

 void NIP_PCF8814::display() {
	for(int p = 0; p < 9; p++){
		if(yUpdateMin >= ((p+1) *8)) {
			continue;
		}
		if(yUpdateMax < p*8){
				break;
		}
		send(PCF8814_CMD, LCD_CHANGE_COL | p); 
		
		int col = xUpdateMin;
		int maxcol = xUpdateMax;
			
		send(PCF8814_CMD, LCD_CHANGE_ROW_LSB | (col % 16));
		send(PCF8814_CMD, LCD_CHANGE_ROW_MSB | (col / 16));
		
		for(; col <= maxcol; col++){
			send(PCF8814_DATA, lcd_memory[(LCDWIDTH * p) + col]);
		}
		
	}
	xUpdateMin = LCDWIDTH - 1;	
	xUpdateMax = 0;
	yUpdateMin = LCDHEIGHT -1;
	yUpdateMax = 0;
}

void NIP_PCF8814::setContrast(byte value)
{
	send(PCF8814_CMD, LCD_VOB_MSB|0x04);
	send(PCF8814_CMD, LCD_VOB_LSB|(value & 0x1F));
}

void NIP_PCF8814::rotate(bool value)
{
	if (value == true)
	{
		send(PCF8814_CMD, LCD_MIRROR_Y|OFF);
		send(PCF8814_CMD, LCD_MIRROR_X|ON);
	}
	else 
	{
		send(PCF8814_CMD, LCD_MIRROR_Y|0x08);
		send(PCF8814_CMD, LCD_MIRROR_X|OFF);
	}
}

void NIP_PCF8814::setInverse(bool invert)
{
	if (invert == true) {
	send(PCF8814_CMD, LCD_MODE|INVERT|ON);
	send(PCF8814_CMD, LCD_MODE|ALL|OFF);
	send(PCF8814_CMD, LCD_MODE|DISPLAY|ON);
	}
	else {
		lcd_on();
	}
}

void NIP_PCF8814::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
    return;

  int16_t t;
  switch(rotation){
    case 1:
      t = x;
      x = y;
      y =  LCDHEIGHT - 1 - t;
      break;
    case 2:
      x = LCDWIDTH - 1 - x;
      y = LCDHEIGHT - 1 - y;
      break;
    case 3:
      t = x;
      x = LCDWIDTH - 1 - y;
      y = t;
      break;
  }
 
  if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
    return;

  // x is which column
  if (color)
    lcd_memory[x+ (y/8)*LCDWIDTH] |= _BV(y%8);
  else
    lcd_memory[x+ (y/8)*LCDWIDTH] &= ~_BV(y%8);

  updateBoundingBox(x,y,x,y);
}

void NIP_PCF8814::send(unsigned char type, unsigned char data)
{
    digitalWrite(pin_DIN, type);
    digitalWrite(pin_CS, LOW);
    digitalWrite(pin_SCLK, HIGH);
    digitalWrite(pin_SCLK, LOW);
    shiftOut(pin_DIN, pin_SCLK, MSBFIRST, data);
    digitalWrite(pin_CS, HIGH);
}

void NIP_PCF8814::lcd_on()
{
	send(PCF8814_CMD, LCD_MODE|DISPLAY|ON);
	send(PCF8814_CMD, LCD_MODE|ALL|OFF);
	send(PCF8814_CMD, LCD_MODE|INVERT|OFF);
}