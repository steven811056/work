#include <Servo.h>

Servo myservo;
int pos;

void setup()
{
	myservo.attach(11);
	//myservo.writeMicroseconds(2000);  // CW
	//delay(5000);
	//myservo.writeMicroseconds(1500);  // stop
	//delay(100);
	//myservo.writeMicroseconds(1800);  // CW
	//delay(5000);
	//myservo.writeMicroseconds(1000);  // CCW
	//delay(3000);
	//myservo.writeMicroseconds(1500);  // stop
	//delay(100);
}

void loop() {
	myservo.writeMicroseconds(1520);  // stop
	delay(100);
	myservo.writeMicroseconds(1800);  // CW
	delay(500);
}