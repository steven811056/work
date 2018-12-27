#include "math.h"
#include "Servo.h"
#include "DeltaRobInverseKin.h"

Servo myservo1;
Servo myservo2;
Servo myservo3;


char Kevin; //轉動方案變數
char Kevin2; //選擇delta或是scara的變數
int delayKevin;
int delayKevin2 = 800; //delay的時間
int degreeKevin = 100;
int a;

void setup()
{
	Serial.begin(9600);
	myservo1.attach(8);
	myservo2.attach(9);
	myservo3.attach(10);
}

void loop() //servo delta 10度放平   100大概變為垂直狀態
{
	int k = 10;
	for (int i = 0; i<10; i++)
	{		
		myservo1.write(3);
		delay(60);
		myservo2.write(k);
		delay(60);
		myservo3.write(k);
		delay(60);
	}
	delay(1000);
	k = 100;
	a = 10;
	for (int i = 0; i<10; i++)
	{
		
		myservo1.write(a);
		delay(500);
		myservo2.write(a);
		delay(500);
		myservo3.write(a);
		delay(500);	
		a = a + 10;
	}
	delay(2000);

	
	/*for (int i2 = 100; i2 > 10; i2--)
	{
		myservo1.write(100);
		delay(15);
		myservo2.write(i2);
		delay(15);
		myservo3.write(i2);
	}
	delay(3000);*/
	

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


