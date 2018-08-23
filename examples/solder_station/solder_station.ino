#include <Adafruit_GFX.h>
#include <NIP_PCF8814.h>
#include <Fonts/Org_01.h>

NIP_PCF8814 lcd(13,12,8,7,9);

#define port_fan      3
#define port_solder   5
#define port_blower   6
#define adc_solder    A6
#define adc_blower    A7

int readtemp1=200;
int readtemp2=300;
int temp1=200;
int temp2=350;
int pwm=200;
float volt=15.5;
float ampere=0.43;

static const unsigned char PROGMEM up[] =
{0x20,0x70,0xF8};
static const unsigned char PROGMEM down[] =
{0xF8,0x70,0x20};
static const unsigned char PROGMEM pas[] =
{0x88,0xF8,0x88};
static const unsigned char PROGMEM up2[] =
{0x20,0x70,0xF8,0x20,0x20};
static const unsigned char PROGMEM down2[] =
{0x20,0x20,0xF8,0x70,0x20};
static const unsigned char PROGMEM pas2[] =
{0x20,0x70,0xF8,0x70,0x20};  
//static const unsigned char PROGMEM charpwm[] =
//{0xF0,0x90,0xF0,0x80,0xA2,0x2A,0x3E,0x00,0xF8,0xA8,0xA8};  

float maping(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(4,OUTPUT);
  analogWrite(3,70);
lcd.begin();
//lcd.clearDisplay();
lcd.setFont(&Org_01);
lcd.setInverse(true);
}

void loop() {
  // put your main code here, to run repeatedly:
readtemp1=map(analogRead(A6),0,1023,0,480);
readtemp2=map(analogRead(A7),0,1023,0,480);
temp1=map(analogRead(A2),0,1023,0,480);
temp2=map(analogRead(A3),0,1023,0,480);
pwm=map(analogRead(A4),0,1023,0,255);
volt=maping(analogRead(A5),0,1023,0,24);
ampere=maping(analogRead(A5),0,1023,0,5);
lcd.clearDisplay();
tampil();delay(10);
}

void tampil(){
lcd.setTextColor(BLACK);

lcd.setTextSize(1);
lcd.setCursor(1,6);
lcd.print("Iron");
if(readtemp2<temp2)     lcd.drawBitmap(1,9,up2,5,5,BLACK);
else if(readtemp2>temp2)lcd.drawBitmap(1,9,down2,5,5,BLACK);
else                    lcd.drawBitmap(1,9,pas2,5,5,BLACK);
lcd.setCursor(7,13);
lcd.print(readtemp2);
lcd.setCursor(31,13);
lcd.print("C");
lcd.setTextSize(2);
lcd.setCursor(28,9);
lcd.print(".");

lcd.setTextSize(1);
lcd.setCursor(1,20);
lcd.print("Hot Air");
//lcd.drawBitmap(1,24,down,5,3,BLACK);
if(readtemp1<temp1)     lcd.drawBitmap(1,23,up2,5,5,BLACK);
else if(readtemp1>temp1)lcd.drawBitmap(1,23,down2,5,5,BLACK);
else                    lcd.drawBitmap(1,23,pas2,5,5,BLACK);
lcd.setCursor(7,27);
lcd.print(readtemp1);
lcd.setCursor(31,27);
lcd.print("C");
lcd.setTextSize(2);
lcd.setCursor(28,23);
lcd.print(".");

lcd.setTextSize(2);
lcd.setCursor(41,11);
lcd.print(">");
lcd.print(temp2);
lcd.setCursor(41,25);
lcd.print(">");
lcd.print(temp1);
lcd.setCursor(86,2);
lcd.print(".");
lcd.setCursor(86,16);
lcd.print(".");

lcd.setTextSize(1);
lcd.setCursor(90,6);
lcd.print("C");
lcd.setCursor(90,20);
lcd.print("C");

lcd.setTextSize(1);
lcd.setCursor(1,38);
lcd.print("Fan");
lcd.setCursor(1,45);
lcd.print("Speed");
//lcd.setCursor(88,37);
//lcd.print("p");
//lcd.setCursor(90,42);
//lcd.print("w");
//lcd.setCursor(88,47);
//lcd.print("m");
lcd.setTextSize(2);
lcd.setCursor(41,43);
lcd.print(">");
int lvl=map(pwm,0,255,0,9);
lcd.print(pwm);
//lcd.drawBitmap(86,34,pwm,7,11,BLACK);
lcd.drawLine(94,45,94,44,BLACK);
lcd.drawLine(90,44,94,44,BLACK);
lcd.drawLine(90,43,90,44-lvl,BLACK);
lcd.drawLine(90,44-lvl,94,44-lvl,BLACK);
lcd.drawLine(94,44-lvl,94,34,BLACK);

lcd.drawLine(0,52,96,52,BLACK);
lcd.drawLine(0,53,96,53,BLACK);
lcd.drawLine(48,52,48,67,BLACK);
lcd.drawLine(47,52,47,67,BLACK);

lcd.setCursor(1,65);
lcd.print(volt,1);
lcd.setCursor(50,65);
lcd.print(ampere,2);

lcd.setTextSize(1);
lcd.setCursor(41,66);
lcd.print("V");
lcd.setCursor(90,66);
lcd.print("A");

lcd.display();
}

