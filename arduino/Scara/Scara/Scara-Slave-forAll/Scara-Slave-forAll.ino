#include <EEPROM.h>
#include <SlaveUnion.h>
#include <Wire.h>
#include <arduino.h>

boolean debug = 1;
//	>>>	for address
int SLAVE_ADDRESS = 0x01;
float f = 0.001;
int eeAddress = 1;
int f1;
//	-------------------------
//	>>>	for motor
int dirPin = A0;
int stepperPin = A1;
const int delaytime = 30;
uint32_t incomingInt = 0;
//	-----------------------
//	>>>	for senser
const int senser = 2;
//	----------------------
//	>>>	for 電磁閥
int suck = 3;
//	------------------
//	>>>	for such
int Show = 0;
String I2C_String = "";
char I2C_Char = '0';
//	---------------------
int8_t end = 0;	//回傳子板使用

void setup()
{
	Serial.begin(115200);
	Wire.begin(SLAVE_ADDRESS);
	Wire.onReceive(test);
	Wire.onRequest(OK);		
	if (debug)
	{
		Serial.println("--> Slave 1 Ready");
	}
	//	>>>	for senser
	pinMode(senser, INPUT);
	digitalWrite(senser, LOW);
	//	----------------------------
	//	>>>	for motor
	pinMode(stepperPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	pinMode(A2, OUTPUT);	//enable_Pin
	digitalWrite(A2, LOW);
	//	-------------------------------
	//	>>>	for suck plate
	pinMode(suck, OUTPUT);	
	end = 1;
	//	-------------------------------
	EEPROM.update(eeAddress,1);	
	f = EEPROM.read(eeAddress);	
	f1 = EEPROM.read(eeAddress);
	Serial.println(f);
	Serial.println(f1);
	EEPROM.get(eeAddress, f1);	
	Serial.println(f1);
	EEPROM.get(eeAddress, f);
	Serial.println(f);
}
UnionTurn testU;

void loop()
{
	if (I2C_String == "start")
	{
		Wire.onReceive(testUU);
	}
	if (I2C_String == "start2")
	{
		Wire.onReceive(testUZ);
	}
	if (I2C_String == "reset")
	{
		Serial.println("reset");
		return1();
		Serial.println("reset  END ");
	}
	if (Show == 1)
	{
		Turn();
	}
	if (Show == 2)
	{
		Turn2();
	}
	//	>>>	slave 0x04
	if (SLAVE_ADDRESS == 0x04)
	{
		if (I2C_String == "open")
		{
			Show = 4;
		}
		else if (I2C_String == "close")
		{
			Show = 5;
		}
		else if (I2C_String == "turn")
		{
			reset_String();
			Show = 6;
		}
	}	
	//	------------------------------------
}

void test(int t)	//	>>>	I2C接收函數
{
	if (debug)
	{
		Serial.println("Slave 1 receive get");
	}
	I2C_String = "";
	while (Wire.available())
	{
		I2C_Char = (char)Wire.read();
		I2C_String = I2C_String + I2C_Char;
		if (debug)
		{
			Serial.println(I2C_String);
		}
	}	
}
//	------------------------------------------------------

void testUU(int a)
{
	testU.Start();
	if (testU.END() == 1)
	{
		Show = 1;
	}
	if (testU.END() == 2)
	{
		Serial.println("--> Nothing ");
	}
}

void testUZ(int a)
{
	testU.Start();
	if (testU.END() == 1)
	{
		Show = 2;
	}
	if (testU.END() == 2)
	{
		Serial.println("--> Nothing ");
	}
}

//	>>>	I2C回傳函數
void OK()	
{
	if (end == 1)
	{
		Wire.write(1);
		end = 0;
	}
}
//	------------------------------------------------------------

void Turn()
{
	if (debug)
	{
		Serial.println("--> Turn1 start");
	}
	incomingInt = testU.incommingByte;
	Serial.println(incomingInt);
	digitalWrite(A2, LOW);
	digitalWrite(dirPin, HIGH);	
	uint32_t i = incomingInt;
	Serial.println(i);
	for (i; i > 0; i = i - 1)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(delaytime);
		/*Serial.print("-->");
		Serial.println(i);*/
	}
	Show = 0;
	end = 1;
	Wire.onReceive(test);
}

void Turn2()
{
	if (debug)
	{
		Serial.println("--> Turn2 start");
	}
	incomingInt = testU.incommingByte;
	Serial.println(incomingInt);
	digitalWrite(A2, LOW);
	digitalWrite(dirPin, LOW);	
	uint32_t i = incomingInt;
	for (i; i > 0; i = i - 1)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(delaytime);
	}	
	Show = 0;
	end = 1;
	Wire.onReceive(test);
}

void return1()
{
	I2C_String = "";
	while (1)
	{
		digitalWrite(dirPin, HIGH);
		if (digitalRead(senser) == HIGH)
		{
			digitalWrite(stepperPin, HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepperPin, LOW);
			delayMicroseconds(delaytime);
		}
		else if (digitalRead(senser) == LOW)
		{
			Serial.println("senser  OK");
			I2C_String = "";
			digitalWrite(dirPin, LOW);
			uint32_t i = (178 * 20) / (0.05625 * 2);
			for (i; i > 0; i = i - 1)
			{
				digitalWrite(stepperPin, HIGH);
				delayMicroseconds(delaytime);
				digitalWrite(stepperPin, LOW);
				delayMicroseconds(delaytime);
			}
			end = 1;
			break;
		}
	}
}

// ----- 轉盤正 OR 反轉
void suck_run_Forward()
{
	Serial.println("forward");
	digitalWrite(dirPin, HIGH);
	incomingInt = testU.incommingByte;
	uint32_t i = incomingInt;
	for (i; i > 0; i = i - 1)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(delaytime);
	}
}

void suck_run_backward()
{
	Serial.println("backward");
	digitalWrite(dirPin, LOW);
	incomingInt = testU.incommingByte;
	uint32_t i = incomingInt;
	for (i; i > 0; i = i - 1)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(delaytime);
	}
}
// -----  **** 轉盤正 OR 反轉
void reset_String()
{
	Serial.println("reset_String");
	I2C_String = "";
}

