/*********************
 *      INCLUDES
 *********************/
 
#include <Servo.h>
#include <Wire.h>  
#include <LiquidCrystal_I2C.h>

/*
 * Initialization Setup for 
 * 16x2 Character LCD Display Module
 * Two 180 degree Servo Motors 
 * Infrared Sensor
 */

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int IR = 2;

Servo servoSort;
Servo servoGate;

int CSS0 = 4; // Colour Sensor
int CSS1 = 5; // Colour Sensor
int CSS2 = 6; // Colour Sensor
int CSS3 = 7; // Colour Sensor
int CSLED = 8; // Colour Sensor
int CSOUT = 3; // Colour Sensor

/*
 * Initialization of Variables and Flags
 */
 
volatile int processFlag = 0;

unsigned int pulseWidth;

long RedLong = 0;
long GreenLong = 0;
long BlueLong = 0;

int RCount = 0;
int GCount = 0;
int BCount = 0;
int OCount = 0;
int TCount = 0;

/*
 * Interrupt Service Routine (ISR) for the IR Sensor
 */

void IRInterrupt()
{
processFlag = 1;
}

/*
 * Arduino's setup to initialize the pins and  
 * peripherals on the board
 */
 
void setup()
{
  
Serial.begin(9600);

pinMode(13,OUTPUT); 

pinMode(IR,INPUT); // Infrared Sensor as input to Pin 2
  
servoSort.attach(9);

servoGate.attach(10);

/*
 * Color Sensor Setup
 */

pinMode(CSS0,OUTPUT);
pinMode(CSS1,OUTPUT);
pinMode(CSS2,OUTPUT);
pinMode(CSS3,OUTPUT);
pinMode(CSOUT,INPUT);

digitalWrite(CSS0,LOW);
digitalWrite(CSS1,HIGH);

/*
 * Test LED on the Color Sensor and Arduino 
 * for confirmation of the object recognition.
 */
 
 // Color Sensor LED 
pinMode(CSLED,OUTPUT);
digitalWrite(CSLED,HIGH);
delay(500);
digitalWrite(CSLED,LOW);

// Arduino LED
digitalWrite(13,LOW);

/*
 * Test two Servo Motors connected to the board 
 */

 // Motor 1 
servoGate.write(65);
delay(1000);
servoGate.write(81);
delay(1000);
servoGate.write(65);
delay(1000);

// Motor 2 
servoSort.write(100);
delay(1000);

for(int i=55;i<=145;i+=30) 
{
  servoSort.write(i);
  delay(1000);
}
servoSort.write(100);
delay(1000);

/*
 * Test and Initialize the LCD connected to the board 
 */

lcd.begin(16,2);
lcd.backlight();

lcd.setCursor(0,0);

lcd.print("   COLOR SORT   ");
lcd.setCursor(0,1);
lcd.print(" HOBBY  PROJECT ");
delay(1500);

lcd.setCursor(0,0);
lcd.print("       BY       ");
lcd.setCursor(0,1);
lcd.print("  ARJUN  WARTY  ");
delay(1500);

lcd.setCursor(0,0);
lcd.print("  DEVELOPED IN  ");
lcd.setCursor(0,1);
lcd.print("      2016      ");
delay(1500);

lcd.setCursor(0,0);
lcd.print(" R  G  B  O  T  "); // Red, Green, Blue, Others and Total
lcd.setCursor(0,1);
lcd.print(" 00 00 00 00 00 ");

interrupts();
attachInterrupt(0,IRInterrupt,RISING); // Edge Triggered Interrupt (Rising Edge)

}

void loop()
{

  if (processFlag == 1)
{
  delay(1000);
  ColourSensorProcess();
  delay(500);
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  processFlag = 0;
}
  
} 

void ColourSensorProcess()

{
  
digitalWrite(CSLED,HIGH);  
delay(250);

/*
 * Color Sensor Pin configuration for Red Color 
 */

digitalWrite(CSS2,LOW);
digitalWrite(CSS3,LOW);
delay(250);
pulseWidth = pulseIn(CSOUT,LOW);
RedLong = pulseWidth;
Red = 255 - pulseWidth/400;
Serial.print("Red : ");
Serial.println(Red);
Serial.print("RedLong : ");
Serial.println(RedLong);

/*
 * Color Sensor Pin configuration for Green Color 
 */

digitalWrite(CSS2,HIGH);
digitalWrite(CSS3,HIGH);
delay(250);
pulseWidth = pulseIn(CSOUT,LOW);
GreenLong = pulseWidth;
Green = 255 - pulseWidth/400 ;
Serial.print("Green : ");
Serial.println(Green);
Serial.print("GreenLong : ");
Serial.println(GreenLong);	

/*
 * Color Sensor Pin configuration for Blue Color 
 */

digitalWrite(CSS2,LOW);
digitalWrite(CSS3,HIGH);
delay(250);
pulseWidth = pulseIn(CSOUT,LOW);
BlueLong = pulseWidth;
Blue = 255 - pulseWidth/400;
Serial.print("Blue : ");
Serial.println(Blue);
Serial.print("BlueLong : ");
Serial.println(BlueLong);

digitalWrite(CSLED,LOW);

/*
 * Color Sensor Values for RGB Pins along with Sorting Conditions
 */
 
if ((RedLong > 1350) && (RedLong < 1700))
{ if ((GreenLong > 2025) && (GreenLong < 2450))
{ if ((BlueLong > 1750) && (BlueLong < 2200))
{
  servoSort.write(85);
  RCount = RCount + 1;
  if (RCount >= 100)
  {
    RCount = 0;
  }
  goto Skip;
}
}
}

if ((RedLong > 1450) && (RedLong < 1900))
{ if ((GreenLong > 1550) && (GreenLong < 1950))
{ if ((BlueLong > 1700) && (BlueLong < 2200))

{
  servoSort.write(115);
  GCount = GCount + 1;
  if (GCount >= 100)
  {
    GCount = 0;
  }
  goto Skip;
}
}
}

if ((RedLong > 1600) && (RedLong < 2100))
{ if ((GreenLong > 1550) && (GreenLong < 2050))
{ if ((BlueLong > 1200) && (BlueLong < 1600))
{
  servoSort.write(145);
  BCount = BCount + 1;
  if (BCount >= 100)
  {
    BCount = 0;
  }
  goto Skip;
}
}
}

servoSort.write(55);
OCount = OCount + 1;
if (OCount >= 100)
{
  OCount = 0;
}

Skip:

/*
 * Total Value reset to 0 once total reaches 100
 */
 
TCount = TCount + 1;
if (TCount >= 100)
{
  TCount = 0;
}

delay(500);
servoGate.write(81);
delay(750);
servoGate.write(65);

/*
 * LCD Display Values
 */
 
lcd.setCursor(0,0);
lcd.print(" R  G  B  O  T  ");
lcd.setCursor(0,1);

lcd.print(" ");

if (RCount < 10)  
{
  lcd.print("0");
  lcd.print(RCount);
}
else
{
  lcd.print(RCount);
}

lcd.print(" ");

if (GCount < 10)  
{
  lcd.print("0");
  lcd.print(GCount);
}
else
{
  lcd.print(GCount);
}

lcd.print(" ");

if (BCount < 10)  
{
  lcd.print("0");
  lcd.print(BCount);
}
else
{
  lcd.print(BCount);
}

lcd.print(" ");

if (OCount < 10)  
{
  lcd.print("0");
  lcd.print(OCount);
}
else
{
  lcd.print(OCount);
}

lcd.print(" ");

if (TCount < 10)  
{
  lcd.print("0");
  lcd.print(TCount);
}
else
{
  lcd.print(TCount);
}

lcd.print(" ");
  
}

