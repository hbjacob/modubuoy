// SLAVE ESP32 BOYE

#define pOUT 33
#define pIN 32

#define rate 100

//importerer sensorbibilotek
#include "AS726X.h"
AS726X sensor;
#include <Wire.h>

#define SDA_PIN 21   // Choose any available GPIO for SDA
#define SCL_PIN 22   // Choose any available GPIO for SCL

String moduleName = "lys";

void setup() {
  // put your setup code here, to run once:
  pinMode(pOUT, OUTPUT);
  pinMode(pIN, INPUT_PULLDOWN);

  Serial.begin(9600);

  Serial.println("");
  Serial.print(moduleName);
  Serial.println(" startup...");

  //til fluoresens sensor
  Wire.begin(SDA_PIN, SCL_PIN);
  sensor.begin();
  sensor.setGain(0);

  delay(1000); // Let states settle after floating values
}

void loop() {
  if(digitalRead(pIN) == HIGH)
  {
    delay(1.5*rate);
    if(digitalRead(pIN) == LOW)
    {
      sensor.takeMeasurements();
      float violet = sensor.getCalibratedViolet();
      byte value = (byte) violet;
      Serial.println(value);
      writeByte(value);
    }
  }
}

void writeByte(byte B)
{
  bool writeBitArray[8];
  byteToBool8(B, writeBitArray);
  for(int i = 7; i >= 0; i--)
  {
    digitalWrite(pOUT, writeBitArray[i]);
    delay(rate);
  }
}

byte bool8toByte(bool bitArray[])
{
  int tall = 0;
  for(int i = 7; i >= 0; i--)
  {
    if(bitArray[i])
    {
      tall += pow(2, i);
    }
  }

  return tall;
}

void byteToBool8(byte B, bool bitArray[8])
{
  //byte Byten = B;
  for(int b = 0; b < 8; b++)
  {
    int t = B / pow(2, 7-b);
    //Serial.print("Tallet t: ");
    //Serial.println(t);

    bitArray[b] = t;
    B -= t*pow(2, 7-b);
  }
}