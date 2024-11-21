// SLAVE ESP32 BOYE

#define pOUT 33
#define pIN 32

#define rate 100

String moduleName = "garb";

int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pOUT, OUTPUT);
  pinMode(pIN, INPUT_PULLDOWN);

  Serial.begin(9600);

  Serial.println("");
  Serial.print(moduleName);
  Serial.println(" startup...");

  delay(1000); // Let states settle after floating values
}

void loop() {
  if(digitalRead(pIN) == HIGH)
  {
    delay(1.5*rate);
    if(digitalRead(pIN) == LOW)
    {
      float val = sin(count*6.28/30)*127 + 127;
      Serial.println(val);
      writeByte( (byte) val); //sin(count*6.28/30)*127 + 127);
      count++;
      count %= 30;
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