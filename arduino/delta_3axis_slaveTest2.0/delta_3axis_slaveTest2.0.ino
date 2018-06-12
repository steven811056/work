#include <Wire.h>

//  AB節臂
#define SlaveAddress 0x01

const int stepPin=2;
const int dirPin=3;

int returnStatus=0;
int Event=0;
boolean reset1;
boolean A_Direction;
double turn[2];
int slave;

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
  if(slave == 1)
  {
    Serial.println("TurnRight");
    turnRight(10);
    slave =0;
  }
  if(slave == 2)
  {
    Serial.println("TurnLeft");
    turnLeft(10);
    slave =0;
  }
  if(slave == 3)
  {
    Serial.println("resetFunction");
    resetFunction();
    slave =0;
  }
  
}

void DegreeTurn()
{ 
  Serial.println("DegreeTurn"); 
  if(Wire.available())
  {
    slave=Wire.read();
    Serial.println(slave);
        
  }  
  
}

void resetFunction()
{
  returnStatus =1;
  Serial.println("reset");
  Serial.print("returnStatus = ");
  Serial.println(returnStatus);
  delay(1000); 
  if(returnStatus ==1)
  {
    requestEvent();
    Event=1;
  }
  
}

void turnRight(int degree)
{
  digitalWrite(dirPin,HIGH);
  delay(30);
  for(int i=0;i<degree;i++) 
  { 
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
}

void turnLeft(int degree)
{
  digitalWrite(dirPin,LOW);
  delay(30);
  for(int i=0;i<degree;i++)
  { 
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
}

void requestEvent()
{ 
  
  if(Event == 1)
  {
    Serial.println("requestEvent");
    Wire.write(1);
    returnStatus =0;
    Event=0;
  }
}

