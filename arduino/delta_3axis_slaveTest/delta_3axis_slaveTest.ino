#include <Wire.h>

//  AB節臂
#define SlaveAddress 1

const int stepPin=2;
const int dirPin=3;

int returnStatus;
boolean reset1;
boolean A_Direction;
double turn[2];
String slave;

void setup() 
{
  Wire.begin(SlaveAddress);
  Wire.onReceive(DegreeTurn);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
  Serial.println("I2C--Salve1---test");
  pinMode(reset1,INPUT);
  
}

void loop()
{
  
  
}

void DegreeTurn()
{  
  while(Wire.available())
  {
    Serial.println(slave);
    slave=Wire.read();
    if(slave == 0)
    {
      turnRight();
    }
    if(slave == 1)
    {
      turnLeft();
    }
    if(slave == 3)
    {
      resetFunction();
    }
  }  
  
}

void resetFunction()
{
  digitalWrite(dirPin,HIGH);
  delay(30);
  while(reset1 == LOW) //  處碰到極限開關
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
  digitalWrite(dirPin,LOW);
  delay(30);
  for(int i=0;i<18;i++) 
  { /*  往極限開關反方向轉18圈，大約2.025度，如果精度是1/16，
    為了讓手臂放開極限開關  */
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
  returnStatus =1;
}

void turnRight()
{
  
}

void turnLeft()
{
  
}

void requestEvent()
{
  if(returnStatus == 1)
  {
    Wire.write(1);
  }
}

