#include <for_String.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <math.h>
#include <Wire.h>
#include <arduino.h>

#define MySerial Serial
#define delay_low_time 2
#define microstep 0.1125

char* choose;	//選擇輸入模式
int32_t turn;		
//
int16_t Max_Speed, Add_Speed;	//最大速度與加速度
double Max_step,add_step;	//轉換成步數
int32_t parabola_constant;	//拋物線方程式	常數
int32_t parabola_x;	//拋物線方程式	X
double add_delaytime;	//加速到目標速度需要的時間
int16_t Max_distance;	//移動距離
double all_delaytime;		//到目標距離需要的時間
//	>>>	馬達腳位
int dirPin = A0;
int stepperPin = A1;
//	<<<	馬達腳位

int add_step_forAdd;
int add_step_compare;
int32_t delaytime;	//每次delay的時間
int32_t all_time;	//經過的總時間
int test=0;
int Serial_input;

void setup()
{
	MySerial.begin(115200);
	pinMode(dirPin, OUTPUT);  
	pinMode(stepperPin, OUTPUT);
	pinMode(A2,OUTPUT);
	pinMode(3, OUTPUT); 
	MySerial.println("start");
	Max_distance = pow(Max_Speed, 2) / 2 / Add_Speed;
}

string_turn for_json_string_;

void loop()
{  
	all_time = 0;
	digitalWrite(A2,LOW);
	digitalWrite(3,HIGH);	
	ForJson();
	if(strcmp(choose, "0") == 0)
	{		
		MySerial.println("choose  -->  0");
		A();        
	}
	if(strcmp(choose, "1") == 0)
	{
		MySerial.println("B");
		B();        
	}
	if (strcmp(choose, "2") == 0)
	{
		MySerial.println("C");
		C();
	}
}

//	>>>	json相關
void json_reset()
{
	choose = "";	
	turn = 0;
}

void ForJson()
{
	json_reset();
forjson_1:
	for_json_string_.String_reset();
	while (for_json_string_.String_end() != 1)
	{
		for_json_string_.String_start();
	}
	MySerial.println("forjson-buffer");
	MySerial.println(for_json_string_.json_string);
	StaticJsonBuffer<150> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(for_json_string_.json_string);
	if (!root.success())
	{
		MySerial.println("forjson-not-success");
		for_json_string_.String_reset();
		goto forjson_1;
	}
	String json_string = root["choose"];
	for (int i = 0; i<sizeof(json_string); i++)
	{
		choose[i] = json_string[i];
	}
	turn = root["turn"];
	turn = (turn * 360)/microstep;
	Max_Speed = root["pps"];
	Add_Speed = root["acc"];	
	MySerial.println(choose);	
	MySerial.println(turn);
	MySerial.println(Max_Speed);
	MySerial.println(Add_Speed);	
	while (MySerial.available() > 0)
	{
		MySerial.read();
	}
}
//	<<<	Forjson

void A()
{
	//add_step_forAdd = 1;	
	if(Max_distance > turn)
	{
		
	}	
	Max_step = Max_Speed / microstep;
	add_step = Add_Speed/ microstep;
	if(add_step-floor(add_step) > 0.3)
	{
		add_step = floor(add_step) +1;
	}
	else
	{
		add_step = floor(add_step);
	}
	//delaytime = 1000000 / (add_step );	
	//add_step_compare = add_step;
	all_delaytime = sqrt((turn * 2) / add_step);
	MySerial.print("all_delaytime  -> ");
	MySerial.println(all_delaytime);
	MySerial.print("add_step  -> ");
	MySerial.println(add_step);
	//	>>>	以每步加一次速度
	double x,y;
	add_delaytime = (Max_step/add_step);
	parabola_x = add_step*(pow(add_delaytime,2)/2);
	parabola_constant = (pow(parabola_x, 2)/4)/ add_delaytime;
	double y2 = (pow(parabola_x, 2) / 4.0 / parabola_constant);
	// ex: double y2= (pow(3200, 2) / 4.0 / 94815.0);
	digitalWrite(dirPin, HIGH);
	delay(10);
	MySerial.print("add_delaytime  -> ");
	MySerial.println(add_delaytime);
	MySerial.print("parabola_x  -> ");
	MySerial.println(parabola_x);
	for(int i= parabola_x;i>=1;i--)	//	>>>	加速階段
	{
		x=i;		
		y = (pow(x, 2) / 4.0 / parabola_constant);		
		delaytime = ((y2-y)*1000000);
		if(delaytime <40)
		{
			delaytime = 40;
		}		
		digital_step();
		y2 = y;
	}

	MySerial.print("等速階段delaytime  -> ");
	MySerial.println(delaytime);
	for(int i=0; i<(turn - parabola_x- parabola_x);i++)
	{
		digital_step();
	}

	y2 =0;
	for (int i =1; i <= parabola_x; i++)	//	>>>	減速階段
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
	//	<<<	
	
	//	>>>	以每秒加一次速度
	//for(int a=0;a<all_delaytime;a++)
	//{
	//	MySerial.print("a  -> ");
	//	MySerial.println(a);	
	//	for(int i=0;i<add_step_compare;i++)
	//	{			
	//		MySerial.print("delaytime  -> ");
	//		MySerial.println(delaytime);			
	//		MySerial.print("all_time  -> ");
	//		MySerial.println(all_time);
	//		all_time = all_time + delaytime;
	//		digital_step();
	//	}
	//	MySerial.print("add_step_compare  -> ");
	//	MySerial.println(add_step_compare);
	//	test++;
	//	MySerial.print("test  -> ");
	//	MySerial.println(test);		
	//	MySerial.print("add_step  -> ");
	//	MySerial.println(add_step);
	//	add_step_compare = add_step_compare + add_step;
	//	delaytime = 1000000 / (add_step_compare);
	//	MySerial.println("------------------------------------");		
	//}		
	
	delay(10);
	Serial_input = '!';
}

void B()
{
	digitalWrite(dirPin, LOW);
	delay(50);
	for (int i = 0; i<3200; i++)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(1873);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(2);
	}
	delay(50);
	Serial_input = '!';
}

void C()
{
	digitalWrite(dirPin, LOW);
	delay(50);
	while(1)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(35);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(2);
	}
	delay(50);
	Serial_input = '!';
}

void digital_step()
{	
	digitalWrite(stepperPin, HIGH);
	delayMicroseconds(delaytime);
	digitalWrite(stepperPin, LOW);
	delayMicroseconds(delay_low_time);
}
