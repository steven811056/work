#include <SlaveUnion.h>
#include <Wire.h>
#include <arduino.h>

#define debug 1
#define Serial_begin Serial.begin(115200); 
#define MySerial Serial
#define delay_low_time 2
#define microstep 0.05625
//#define microstep 0.1125
#define Slave_Address 0x01

char incomingByte = '0';
int incommingint;
String incomingString = "";
//
int16_t Max_Speed ;	//最大速度與加速度
int32_t Add_Speed;
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
int32_t delaytime = 200;
int Show = 0;

void setup()
{
	Serial_begin;
	Wire.begin(Slave_Address);
	Wire.onReceive(number);
	Wire.onRequest(status);
	pinMode(A2, OUTPUT);
	if (debug)
	{
		Serial.print("Slave ");
		Serial.print(Slave_Address);
		Serial.println("--> Ready");
	}
	pinMode(senser, INPUT);
	pinMode(stepperPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	pinMode(A2, OUTPUT);
	digitalWrite(senser, LOW);
}

UnionTurn delta_Announce(true);

void loop()
{
	if (incomingString == "reset")
	{
		Serial.println("reset");
		return1();
		Serial.println("reset  END ");
	}
	if (incomingString == "E_Stop")
	{
		Serial.println("E_Stop");
		E_Stop();
	}
	if (incomingString == "start")
	{
		Serial.println("start");
		Serial.println(turn);
		digitalWrite(A2, LOW);
		digitalWrite(dirPin, HIGH);
		if (turn != 0)
		{
			before_step();
		}		
		incomingString = "";
	}
	if (incomingString == "start2")
	{
		Serial.println("start2");
		Serial.println(turn);
		digitalWrite(A2, LOW);
		digitalWrite(dirPin, LOW);
		if (turn != 0)
		{
			before_step();
		}
		incomingString = "";
	}
}

//	>>>	I2C呼叫跟回傳
void number(int a)
{
	if (Wire.available())
	{
		Serial.print("** onReceive_fun_number = ");
		Serial.println(Wire.available());
		incommingint = Wire.read();
		Serial.print("Contral number = ");
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
		if (direction == 1)
		{
			incomingString = "start";
		}
		else if (direction == 2)
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
	case 6:
		incomingString = "E_Stop";
	case 7:
		incomingString = "E_Start";
	default:
		break;
	}
}

void status()
{

}
//	<<<	I2C呼叫跟回傳

//	>>>>>		
void before_step()
{
	boolean debug_for_step;
	debug_for_step = 0;	// for  debug
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
	add_delaytime = (Max_step / add_step);
	if (pow(add_delaytime,2)*(add_step) > turn)
	{
		double area = (turn/2);
		up(area, debug_for_step);
		down(area, debug_for_step);
	}
	else
	{
		double area, same_area;
		area = (add_delaytime * Max_step) / 2;
		same_area = turn - (area * 2);
		up(area, debug_for_step);
		same(area, debug_for_step);
		down(area, debug_for_step);
	}	
}

void up(double area,boolean debug_up)
{	
	double all =0;
	if (debug_up)
	{
		MySerial.print("UP  -> ");
		MySerial.println(area);
	}	
	double time[2] = { 0,0 };	
	time[0] = sqrt((2 * 1) / add_step) * 1000000;
	for (int i = 2; i <= area; i++)
	{
		time[1] = sqrt((2 * i) / add_step) * 1000000;
		delaytime = (time[1] - time[0]) ;
		if (debug_up)
		{
			MySerial.print("time1  -> ");
			MySerial.println(time[1]);
			MySerial.print("time0  -> ");
			MySerial.println(time[0]);
			MySerial.print("delaytime  -> ");
			MySerial.println(delaytime);
			all = all + delaytime;
		}		
		delay_top();
		digital_step();
		time[0] = time[1];		
	}
	if (debug_up)
	{
		MySerial.print("UP  All-> ");
		MySerial.println(all);
	}	
}

void same(double area, boolean debug_same)
{
	double all = 0;
	if (debug_same)
	{
		MySerial.print("same  -> ");
		MySerial.println(turn - (area * 2));
		MySerial.print("delaytime  -> ");
		MySerial.println(delaytime);
	}	
	delay_top();
	for (int i = 0; i < (turn - (area * 2)); i++)
	{			
		digital_step();
		all = all + delaytime;
	}
	if (debug_same)
	{
		MySerial.print("same  All-> ");
		MySerial.println(all);
	}	
}

void down(double area, boolean debug_down)
{
	double all = 0;
	if (debug_down)
	{
		MySerial.print("down  -> ");
		MySerial.println(area);
	}	
	double time[2] = { 0,0 };
	time[0] = sqrt((2 * area) / add_step) * 1000000;
	for (int i = (area-1); i >= 0; i--)
	{
		time[1] = sqrt((2 * i) / add_step) * 1000000;
		delaytime = (time[1] - time[0]);
		if (delaytime < 0)
		{
			delaytime = (-1)*delaytime;
		}
		delay_top();
		if (debug_down)
		{
			MySerial.print("time1  -> ");
			MySerial.println(time[1]);
			MySerial.print("time0  -> ");
			MySerial.println(time[0]);
			MySerial.print("delaytime  -> ");
			MySerial.println(delaytime);
			all = all + delaytime;
		}		
		digital_step();
		time[0] = time[1];		
	}
	if (debug_down)
	{
		MySerial.print("down  All-> ");
		MySerial.println(all);
	}	
}

void delay_top()
{
	int top = 30;
	if (delaytime < 30)
	{
		delaytime = 30;
	}
}

void digital_step()	//步進馬達控制
{
	digitalWrite(stepperPin, HIGH);
	delayMicroseconds(delaytime);
	digitalWrite(stepperPin, LOW);
	delayMicroseconds(delay_low_time);
}
//	<<<<<	

void E_Stop()	//緊急停止
{
	do
	{
		incomingString = "";
	} while (incomingString != "E_Start");
	MySerial.println("E_Stop --> END");
}

void return1()	//復歸原點
{
	delaytime = 200;
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
