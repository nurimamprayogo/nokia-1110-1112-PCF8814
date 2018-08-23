#ifndef _NIP_PCF8814_H
#define _NIP_PCF8814_H


#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#define BLACK 1
#define WHITE 0
#define LCDWIDTH 96
#define LCDHEIGHT 69
#define PCF8814_CMD  LOW
#define PCF8814_DATA HIGH
#define LCD_CONTRAST 0x15
#define ON 0x01
#define OFF 0x00
#define ALL 0x04
#define INVERT 0x06
#define DISPLAY 0x0E
#define LCD_NOP 0xE3
#define LCD_MODE 0xA0
#define LCD_VOB_MSB 0x20
#define LCD_VOB_LSB 0x80
#define LCD_CHARGE_PUMP_ON 0x2F
#define LCD_RAM_ADDR_MODE 0xAA
#define LCD_CHANGE_ROW_LSB 0x00
#define LCD_CHANGE_ROW_MSB 0x10
#define LCD_CHANGE_COL 0xB0
#define LCD_MIRROR_Y 0xC0
#define LCD_MIRROR_X 0xA0
#define LCD_EXT_OSC 0x3A
#define LCD_SOFT_RESET 0xE2
#define LCD_FRAME_FREQ 0xEC				
#define LCD_PARTIAL_DISPLAY 0xD0

#include <Adafruit_GFX.h>

class NIP_PCF8814 : public Adafruit_GFX {
 public:
  // Software SPI with explicit CS pin.
	NIP_PCF8814(char SCLK, char DIN, char CS, char RST ,char BL);
	NIP_PCF8814(char SCLK, char DIN, char CS, char RST );
	void begin();
	void setContrast(byte value);
	void clearDisplay();
	void clearLine();
	void display();
	void rotate(bool value); 
	void setInverse(bool invert);
	void setBrightness(unsigned char _bl){analogWrite(pin_BL,_bl);}
	void drawPixel(int16_t x, int16_t y, uint16_t color);
	uint8_t getPixel(int8_t x, int8_t y);
		
 private:
	unsigned char pin_SCLK,pin_DIN,pin_CS,pin_RST,pin_BL;
	void send(unsigned char type, unsigned char data);
	void lcd_on();
	unsigned char column;
    unsigned char line;
};

#endif