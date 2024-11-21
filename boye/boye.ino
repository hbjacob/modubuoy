// MASTER ESP32 BOYE

#define pOUT1 32
#define pIN1 33
#define pOUT2 19
#define pIN2 18

#define RATE 100
#define timeoutMillis 20000

#include <WiFi.h>

char ssid[] = "boye";
char pass[] = "roottoor";
char addr[] = "192.168.137.1";
int port = 5000;
WiFiClient client;

bool moduleConnected[2] = {false, false};
int numModules = 2; // Length of moduleConnected
int numVariables[2] = {0, 0}; // Number of variables for each module connected
String varNames[2][2];
int values[2]; // The values to broadcast

byte rConn = 0xA0; // Command to request connection
byte rValues = 0xB1; // Command to request value

unsigned long tsLastCheck[] = {0, 0}; //timestamp of the last time module said hi

void setup() {
  // put your setup code here, to run once:
  pinMode(pOUT1, OUTPUT);
  pinMode(pIN1, INPUT_PULLDOWN);
  pinMode(pOUT2, OUTPUT);
  pinMode(pIN2, INPUT_PULLDOWN);

  Serial.begin(9600);
  Serial.println("");
  Serial.println("Boye started...");

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, pass);
  
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("Connected to the WiFi network");

  Serial.println("Connecting to the server...");
  client.connect(addr, port);
  Serial.println("Connected to the server");
}












void loop() {

  for(int m = 0; m < numModules; m++)
  {
    values[m] = readByte(m);
    Serial.println(values[m]);
    client.write( (byte) values[m]);
  }
}







// BOYE-INTERFACE
void writeByte(byte B, int m)
{
  bool writeBitArray[8];
  byteToBool8(B, writeBitArray);
  for(int i = 7; i >= 0; i--)
  {
    delay(RATE);
    if(m == 0)
    {
      digitalWrite(pOUT1, writeBitArray[i]);
    }
    else if(m == 1)
    {
      digitalWrite(pOUT2, writeBitArray[i]);
    }
  }
}

byte readByte(int m)
{
  bool bitList[8];

  if(m == 0)
  {
    digitalWrite(pOUT1, HIGH);
    delay(RATE);
    digitalWrite(pOUT1, LOW);

    for(int i = 0; i < 8; i++)
    {
      delay(RATE);

      bitList[i] = digitalRead(pIN1);

    }

    return bool8toByte(bitList);
  }
  else if(m == 1)
  {
    digitalWrite(pOUT2, HIGH);
    delay(RATE);
    digitalWrite(pOUT2, LOW);

    for(int i = 0; i < 8; i++)
    {
      delay(RATE);

      bitList[i] = digitalRead(pIN2);

    }

    return bool8toByte(bitList);
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

/* Boye spesific functions

void requestValue(int m)
{
  if(m == 0)
  {
    //Serial.print(".");
    if(digitalRead(pIN1) == HIGH)
    {
      delay(RATE*1.5);
      if(digitalRead(pIN1) == LOW)
      {
        writeByte(rValues, m);

        for(int j = 0; j < numVariables[m]; j++)
        {
          Serial.print(numVariables[m]);
          values[m][j] = readByte(m);
          Serial.print("value : ");
          //Serial.print(values[0][0], HEX);
          Serial.println(values[m][j], HEX);
        }
        sendValuesToServer();
        tsLastCheck[m] = millis();
      }
      else
      {

      }
    }
  }
  else if(m == 1)
  {
    //Serial.print(".");
    if(digitalRead(pIN2) == HIGH)
    {
      delay(RATE*1.5);
      if(digitalRead(pIN2) == LOW)
      {
        writeByte(rValues, m);

        for(int j = 0; j < numVariables[0]; j++)
        {
          values[m][j] = readByte(m);
        }
        Serial.print("value : ");
        //Serial.print(values[0][0], HEX);
        Serial.println(values[0][0], HEX);
        sendValuesToServer();
        tsLastCheck[m] = millis();
      }
      else
      {

      }
    }
  }
}

void connectModule(int m) // m : module nr.
{
  if(m == 0)
  {
    if(digitalRead(pIN1) == HIGH)
    {
      delay(RATE*1.5);
      if(digitalRead(pIN1) == LOW)
      {
        // Connected module has been recognized
        // Start by requesting number of variables
        writeByte(rConn, m);
        Serial.println("Requesting connection...");

        // Retrieve number of variables
        numVariables[m] = readByte(m);
        Serial.print("Retrieved: Number of variables: ");
        Serial.println(numVariables[m]);

        // Request the variable names
        for(int n = 0; n < numVariables[m]; n++)
        {
          // After number of variables has been retrieved, there will be a byte retrieved for the length of the variable name
          int lenVarName = readByte(m);
          Serial.print("Length of variable: ");
          Serial.println(lenVarName);
          for(int ch = 0; ch < lenVarName; ch++)
          {
            varNames[m][n] += (char)readByte(m);
            //Serial.println((char)readByte());
          }
          Serial.println(varNames[m][n]);
        }

        // Connection established, set moduleConnected -> true
        moduleConnected[m] = true;
        tsLastCheck[m] = millis();
      }
    }
  }
  else if(m == 1)
  {
    if(digitalRead(pIN2) == HIGH)
    {
      delay(RATE*1.5);
      if(digitalRead(pIN2) == LOW)
      {
        // Connected module has been recognized
        // Start by requesting number of variables
        writeByte(rConn, m);
        Serial.println("Requesting connection...");

        // Retrieve number of variables
        numVariables[m] = readByte(m);
        Serial.print("Retrieved: Number of variables: ");
        Serial.println(numVariables[m]);

        // Request the variable names
        for(int n = 0; n < numVariables[m]; n++)
        {
          // After number of variables has been retrieved, there will be a byte retrieved for the length of the variable name
          int lenVarName = readByte(m);
          Serial.print("Length of variable: ");
          Serial.println(lenVarName);
          for(int ch = 0; ch < lenVarName; ch++)
          {
            varNames[m][n] += (char)readByte(m);
            //Serial.println((char)readByte());
          }
          Serial.println(varNames[m][n]);
        }

        // Connection established, set moduleConnected -> true
        moduleConnected[m] = true;
        tsLastCheck[m] = millis();
      }
    }
  }
}

void sendValuesToServer()
{
  client.write( (byte) values[0][0]);
  client.write( (byte) values[0][1]);
  client.write( (byte) values[1][0]);
  client.write( (byte) values[1][1]);
}
*/