#include <Wire.h>

#define SlaveAddress 1

const int stepPin[3] = {2, 4, 6};
const int dirPin[3] = {3, 5, 7};
boolean A_Direction;
double turn[2];
String slave;

void setup() 
{
  Wire.begin(SlaveAddress);
  Wire.onReceive(DegreeTurn);
  Serial.begin(9600);
  Serial.println("I2C--Salve1---test");
  
}

void loop() 
{
  
  
}

void DegreeTurn(int test)
{  
  while(Wire.available())
  {
    Serial.println(slave);
    if(slave=="A_Direction");
    {
      
    }
  }
  
  for(int i=0;i<int(turn[1]);i++) //b點旋轉
  {
    digitalWrite(stepPin[1], HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin[1], LOW);
    delayMicroseconds(800);
  }
}

