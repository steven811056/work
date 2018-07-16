#include <Wire.h>

//  AB節臂
#define SlaveAddress 0x01

const int stepPin=2;
const int dirPin=3;

int returnStatus=0;
int Event=0;
int reset1=8;
boolean A_Direction;
double turn[2];
int slave;
//int slave3[4][1]; //讓I2C在傳輸之前先給資料型態存在第一維度
                //0,1,2,3 -> 0為int, 1為float, 2為double, 3為char
int slave3; 
String slave2;
double test1,test2;

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
 /* if(slave == 1)
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
  }*/
  
}

void DegreeTurn()
{   
  Serial.println("DegreeTurn");   
  if(Wire.available())
  {  
    while(Wire.available())
    {
		slave2=slave2+(char)Wire.read(); 
    }
    if(slave2 == "kevin1")
    {
		Serial.println("kevin1");
		slave2="";
    }
    if(slave2 == "kevin2")
    {
		//Wire.onReceive(DegreeTurn2);	//在程式執行之中可以改變onReceive呼叫的函式
		//當程式執行到這個Wire.onReceive，會先執行完上次呼叫的receive的函式，
		//下次呼叫onReceive時就會開始執行另一個函式
		Serial.println("kevin2");
		slave2="";
    }
  }    
    
}

//DegreeTurn2 -- For test
void DegreeTurn2()
{
	Serial.println("123");
	if (Wire.available())
	{
		Serial.println("321");
		while (Wire.available())
		{
			slave2 = Wire.read();
			slave2 = "";
			
		}
		
	}
}

void resttest()
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
}

void resetFunction()
{
//  digitalWrite(dirPin,HIGH);
//  delay(30);
//  while(reset1 == LOW) //  處碰到極限開關
//  {
//    digitalWrite(stepPin, HIGH);
//    delayMicroseconds(500);
//    digitalWrite(stepPin, LOW);
//    delayMicroseconds(500);
//  }
//  digitalWrite(dirPin,LOW);
//  delay(30);
//  for(int i=0;i<18;i++) 
//  { /*  往極限開關反方向轉18圈，大約2.025度，如果精度是1/16，
//    為了讓手臂放開極限開關  */
//    digitalWrite(stepPin, HIGH);
//    delayMicroseconds(500);
//    digitalWrite(stepPin, LOW);
//    delayMicroseconds(500);
//  }
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

