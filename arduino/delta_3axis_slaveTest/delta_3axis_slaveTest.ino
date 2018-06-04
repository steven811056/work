#include <Wire.h>

#define SlaveAddress 1

void setup() 
{
  Wire.begin(SlaveAddress);
  Wire.onReceive(turn);
  Serial.println("I2C--Salve1---test");
  
}

void loop() 
{
 

}

void turn()
{
  
}

