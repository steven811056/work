#include <Wire.h>
#include <LiquidCrystal_I2C.h>  

#define SENSOR_Pin  2 //Interrupt PIN only D2/D3
#define SENSOR2_Pin 3 //Interrupt PIN only D2/D3
#define MA       5   //馬達
#define MB       6

int ENC_VAL;
unsigned long timmer,Atest2=0;
int Atest=0,Afinal=0;
int A=0;

void setup() 
{ 
  pinMode(SENSOR_Pin,INPUT);//2     Interrupt PIN only D2/D3  外部中斷腳位  只有2跟3
  pinMode(SENSOR2_Pin,INPUT);//3    Interrupt PIN only D2/D3
  pinMode(MA,OUTPUT);//5    馬達
  pinMode(MB,OUTPUT);//6    馬達
  digitalWrite(MA,LOW);
  digitalWrite(MB,LOW); 
  Serial.begin(9600);  
  attachInterrupt(digitalPinToInterrupt(SENSOR_Pin),Encoder1,FALLING); //中斷 (設定中斷的腳位，連接到Encoder函數，由電位由HIGH->LOW) 
  Serial.println("Start");
//  setPwmFrequency(5, 1);
}

String inString="";
char inChar;


void loop() 
{     
  if(Serial.available())
  {                   
      while(Serial.available() > 0)
      {          
          inChar = Serial.read();
          if (isDigit(inChar)) 
          {  //isDigit 檢查是否是數字(字元0~9)
            inString += inChar;
          }
          if(inChar=='R' || inChar=='r')
          {
            CC(255);  // 正轉(全速)            
            timmer=micros();
          }
          
          if(inChar=='B' || inChar=='b')
          {
            CCW(255); //反轉(全速)
            timmer=micros();
          }
          if(inChar=='C' || inChar=='c')
          {
            stop1();
            Serial.println(micros()-timmer);
            Serial.println(Atest);             
          }          
      }//while(Serial.available() > 0)             
  }//if(Serial.available())
  if(Atest >= 1446)
  {
    Afinal=Afinal+1;
    Atest=0;
    A=1;
  }
  //if(Atest >= 11000)
  double q=0;
  if(Afinal >= 1 && A>0)
  {
      stop1();
      q=micros()-timmer;
      Serial.print(q/1000000.0);      
      Serial.println(Afinal);
      Serial.println(Atest);
      Serial.println(Atest2);
      A=0;   
  }          
  
}//void loop() 

void CC(byte S){  //全速正轉
  //byte -->  A byte stores an 8-bit unsigned number, from 0 to 255.
  digitalWrite(MA,LOW); //輸入TA7291P 5腳位 低電位 0V  PD4
  digitalWrite(MB,LOW); //輸入TA7291P 6腳位 低電位 0V  PD5  
  delay(500);
  analogWrite(MA,S); //輸入TA7291P 5腳位 高電位 5V
//  digitalWrite(MA,HIGH);
//  analogWrite(MB,255);
  digitalWrite(MB,LOW);
  Serial.print("PWM5腳位: ");
  Serial.println(S);
  
}

void CCW(byte S){ //全速反轉
  digitalWrite(MA,LOW);
  digitalWrite(MB,LOW);
  delay(500);
  digitalWrite(MB,HIGH);  //輸入TA7291P 6腳位 高電位 5V
  digitalWrite(MA,LOW);
  Serial.print("數位輸出6腳位: ");
  Serial.println(HIGH);
  
}

void stop1()// 1,1  煞車  0,0->停止
{
  digitalWrite(MA,HIGH);
  digitalWrite(MB,HIGH);
}

void Encoder1()
{
  Atest=Atest+1;
  Atest2+=1;
//  Serial.println(Atest);
}


/*
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}*/
