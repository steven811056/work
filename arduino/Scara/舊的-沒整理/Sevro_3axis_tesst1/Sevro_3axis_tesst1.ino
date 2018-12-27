#include "math.h"
#include "Servo.h"

Servo myservo1;
Servo myservo2;
Servo myservo3;

const int dirPin[3] = { 3,5,7 };    //A4988的dirPin腳位宣告
const int stepPin[3] = { 2,4,6 };   //A4988的stepPin腳位宣告

char Kevin; //轉動方案變數
char Kevin2; //選擇delta或是scara的變數
int delayKevin;
int delayKevin2 = 800; //delay的時間
int degreeKevin = 100;

void setup()
{
	Serial.begin(9600);	
	myservo1.attach(8);
	myservo2.attach(9);
	myservo3.attach(10);
}

void loop() //servo delta 10度放平   100大概變為垂直狀態
{
  int k=100; 
  for(int i=0;i<50;i++)
  {
    myservo1.write(k);
    delay(15); 
    myservo2.write(k);
    delay(15); 
    myservo3.write(k);
    delay(15); 
  }
  
//	for (int i = 50; i < degreeKevin; i++)
//	{
//		myservo1.write(i); 
//		if (i < (degreeKevin/2))
//		{
//			myservo2.write(i);
//			myservo3.write(i);
//		}
//		
//		delay(20);
//	}
//	for(int i=degreeKevin;i >50 ; i-- )
//	{
//		myservo1.write(i);
//		if (i > (degreeKevin / 2))
//		{
//			myservo2.write(i);
//			myservo3.write(i);
//		}
//		delay(20);
//	}
//	for (int i = 0; i < degreeKevin; i++)
//	{
//		myservo2.write(i); // 使用write，傳入角度，從0度轉到180度
//		if (i < (degreeKevin / 2))
//		{
//			myservo1.write(i);
//			myservo3.write(i);
//		}
//
//		delay(20);
//	}
//	for (int i = degreeKevin; i >0; i--)
//	{
//		myservo2.write(i);
//		if (i > (degreeKevin / 2))
//		{
//			myservo1.write(i);
//			myservo3.write(i);
//		}
//		delay(20);
//	}
//	for (int i = 0; i < degreeKevin; i++)
//	{
//		myservo3.write(i); // 使用write，傳入角度，從0度轉到180度
//		if (i < (degreeKevin / 2))
//		{
//			myservo2.write(i);
//			myservo1.write(i);
//		}
//
//		delay(20);
//	}
//	for (int i = degreeKevin; i >0; i--)
//	{
//		myservo3.write(i);
//		if (i > (degreeKevin / 2))
//		{
//			myservo2.write(i);
//			myservo1.write(i);
//		}
//		delay(20);
//	}
}


