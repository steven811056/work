#include <SlaveUnion.h>
#include <Wire.h>
#include <arduino.h>

#define debug 1
#define Serial_begin Serial.begin(115200); 
#define MySerial Serial
#define delay_low_time 2
#define microstep 0.1125

int SLAVE_ADDRESS = 0x01;
char incomingByte = '0';
int incommingint;
String incomingString = "";
//
int16_t Max_Speed, Add_Speed;	//最大速度與加速度
double Max_step, add_step;	//轉換成步數
int32_t parabola_constant;	//拋物線方程式	常數
int32_t parabola_x;	//拋物線方程式	X
double add_delaytime;	//加速到目標速度需要的時間
int16_t Max_distance;	//移動距離
double all_delaytime;		//到目標距離需要的時間
int32_t turn;		//到目標的步數
//
int direction;		//方向儲存位置
//	>>>	控制板腳位相關
int dirPin = A0;
int stepperPin = A1;
const int senser = 2;
//	<<<	控制板腳位相關
uint32_t incomingInt = 0;
int delaytime = 250;
int Show = 0;


void setup()
{	
	Serial_begin;	
	Wire.begin(SLAVE_ADDRESS);
	Wire.onReceive(number);
	pinMode(A2, OUTPUT);
	if (debug)
	{
		Serial.println("--> Slave 1 Ready");
	}
	pinMode(senser, INPUT);
	pinMode(stepperPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	pinMode(A2, OUTPUT);
	digitalWrite(senser, LOW);	
}

UnionTurn delta_Announce;

void loop()
{
	if (incomingString == "reset")
	{
		Serial.println("reset");
		return1();
		Serial.println("reset  END ");
	}
	if (incomingString == "start")
	{
		Serial.println("start");
		Serial.println(turn);
		digitalWrite(A2, LOW);
		digitalWrite(dirPin, HIGH);
		before_step();
		incomingString = "";
	}
	if (incomingString == "start2")
	{
		Serial.println("start2");
		Serial.println(turn);
		digitalWrite(A2, LOW);
		digitalWrite(dirPin, LOW);
		before_step();
		incomingString = "";
	}	
}

void number(int a)
{	
	if (Wire.available())
	{
		Serial.print("onReceive	");
		Serial.println(Wire.available());
		incommingint = Wire.read();
		Serial.println(incommingint);
	}
	switch (incommingint)
	{
	case 0:
		incomingString = "reset";
		break;
	case 1:
		delta_Announce.Start();
		direction = delta_Announce.incommingByte;
		if(direction == 1)
		{
			incomingString = "start";
		}
		else if(direction == 2)
		{
			incomingString = "start2";
		}
		break;
	case 2:
		delta_Announce.Start();
		turn = delta_Announce.incommingByte;
		break;
	case 3:
		delta_Announce.Start();
		incomingString = "Max";
		Max_Speed = delta_Announce.incommingByte;
		Serial.println(incomingString);
		Serial.println(Max_Speed);
		break;
	case 4:
		delta_Announce.Start();
		incomingString = "acc";
		Add_Speed = delta_Announce.incommingByte;
		Serial.println(incomingString);
		Serial.println(Add_Speed);
		break;
	case 5:
		break;
	default:
		break;
	}	
}

//	>>>>>		
void before_step()
{		
	Max_step = Max_Speed / microstep;
	add_step = Add_Speed / microstep;
	if (add_step - floor(add_step) > 0.3)
	{
		add_step = floor(add_step) + 1;
	}
	else
	{
		add_step = floor(add_step);
	}	
	all_delaytime = sqrt((turn * 2) / add_step);
	MySerial.print("all_delaytime  -> ");
	MySerial.println(all_delaytime);
	MySerial.print("add_step  加速度-> ");
	MySerial.println(add_step);
	//	>>>	以每步加一次速度
	double y2;	//	拋物線方程式 開口向上 X^2 = 4C Y 
	add_delaytime = (Max_step / add_step);
	//以a為加速度所走的距離為parabola_x
	parabola_x = add_step * (pow(add_delaytime, 2) / 2);	//x=(1/2)at^2
	parabola_constant = (pow(parabola_x, 2) / 4) / add_delaytime;	//常數C
	y2 = (pow(parabola_x, 2) / 4.0 / parabola_constant);
	if(add_step > turn)
	{
		delaytime = 40;
		for(int i =turn ;i>0;i--)
		{
			digital_step();
		}		
	}
	movement( y2);
}

void movement( double y2)
{
	double x,y;	//	拋物線方程式 開口向上 X^2 = 4C Y 		
	add_delaytime = (Max_step / add_step);
	//以a為加速度所走的距離為parabola_x
	parabola_x = add_step * (pow(add_delaytime, 2) / 2);	//x=(1/2)at^2
	parabola_constant = (pow(parabola_x, 2) / 4) / add_delaytime;	//常數C		
	MySerial.print("add_delaytime  -> ");
	MySerial.println(add_delaytime);
	MySerial.print("parabola_x  -> ");
	MySerial.println(parabola_x);
	for (int i = parabola_x; i >= 1; i--)	//	>>>	加速階段
	{
		x = i;
		y = (pow(x, 2) / 4.0 / parabola_constant);
		delaytime = ((y2 - y) * 1000000);
		if (delaytime <40)
		{
			delaytime = 40;
		}
		digital_step();
		y2 = y;
	}
	MySerial.print("等速階段delaytime  -> ");
	MySerial.println(delaytime);
	for (int i = 0; i<(turn - parabola_x - parabola_x); i++)
	{
		digital_step();
	}
	y2 = 0;
	MySerial.print("減速階段delaytime  -> ");
	MySerial.println(delaytime);
	for (int i = 1; i <= parabola_x; i++)	//	>>>	減速階段
	{
		x = i;
		y = (pow(x, 2) / 4.0 / parabola_constant);
		delaytime = ((y - y2) * 1000000);
		if (delaytime <30)
		{
			delaytime = 30;
		}
		digital_step();
		y2 = y;
	}
}

void digital_step()
{
	digitalWrite(stepperPin, HIGH);
	delayMicroseconds(delaytime);
	digitalWrite(stepperPin, LOW);
	delayMicroseconds(delay_low_time);
}
//	<<<<<		

void return1()
{
	incomingString = "";
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
		if (digitalRead(senser) == LOW)
		{
			Serial.println("senser  OK");
			incomingString = "";
			break;
		}
	}
}
