#include <arduino.h>
#include <Wire.h>

#define MySerial Serial
#define myserial MySerial
#define p(aa) MySerial.print(aa);
#define pl(aa) MySerial.println(aa);
#define debug 1

int Show;
int32_t For_union = 0 ;
int Array_Long;

union unionType
{
	byte a[8] = { 0,0,0,0,0,0,0,0 };
	int32_t b;
};

void setup()
{
	Wire.begin();
	MySerial.begin(115200);
	MySerial.println("**  Start **");
}

void loop() 
{
	control();
	if (Show == 1)
	{		
		unionType slave1;
		slave1.b = For_union;
		Serial.println(slave1.b);
		Array_Long = (sizeof(slave1.a)) / (sizeof(slave1.a[0]));
		for (int i; i < Array_Long; i++)
		{
			Serial.print("slave1.a = ");
			Serial.println(slave1.a[i]);
		}		
		Show = 0;
		For_union = 0;
	}
}

void control()
{
	/*while (1)
	 {
		if (Serial.available())
		{			
			For_union = Serial.parseInt();		
			break;
		}
	 }*/
	For_union = 500000;
	if (For_union > 0)
	{
		Show = 1;
	}
}
