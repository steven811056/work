#include <SlaveUnion.h>
#include <Wire.h>
#include <arduino.h>

#define debug 1
#define Serial_begin Serial.begin(115200); 

int SLAVE_ADDRESS = 0x01;
char incomingByte = '0';
String incomingString = "";
//
int16_t Max_Speed, Add_Speed;	//�̤j�t�׻P�[�t��
double Max_step, add_step;	//�ഫ���B��
int32_t parabola_constant;	//�ߪ��u��{��	�`��
int32_t parabola_x;	//�ߪ��u��{��	X
double add_delaytime;	//�[�t��ؼгt�׻ݭn���ɶ�
int16_t Max_distance;	//���ʶZ��
double all_delaytime;		//��ؼжZ���ݭn���ɶ�
				//
				//	>>>	����O�}�����
int dirPin = A0;
int stepperPin = A1;
const int senser = 2;
//	<<<	����O�}�����
uint32_t incomingInt = 0;
const int delaytime = 250;
int Show = 0;
int direction;

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
	if (incomingString == "acc")
	{
		Wire.onReceive(number);
		if(delta_Announce.END() == 1)
		{
			Add_Speed = delta_Announce.incommingByte;
			Serial.println(Add_Speed);
		}		
	}
}

void number(int a)
{
	delta_Announce.Start();
}
