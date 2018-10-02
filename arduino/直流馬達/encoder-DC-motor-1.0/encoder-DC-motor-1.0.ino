#include <Wire.h>
#include <LiquidCrystal_I2C.h>  

#define SENSOR_Pin  2 //Interrupt PIN only D2/D3
#define SENSOR2_Pin 3 //Interrupt PIN only D2/D3
#define MA       4   //馬達
#define MB       5
#define M_PWM    6   // 馬達控速
#define SW1      8    // I2C 位置設定
#define SW2      9    // I2C 位置設定
#define SW3      10   // I2C 位置設定
#define SW4      11   // I2C 位置設定
#define LED      A0
#define M0_VR    A1   // VR 訊號腳位
#define SPD_LOW  90  //速度PWM值下限

int ENC_VAL;
unsigned long timmer,Atest2=0;
int Atest=0,Afinal=0;

void setup() 
{ 
  pinMode(SENSOR_Pin,INPUT);//2     Interrupt PIN only D2/D3  外部中斷腳位  只有2跟3
  pinMode(SENSOR2_Pin,INPUT);//3    Interrupt PIN only D2/D3
  pinMode(SW1,INPUT);//8    I2C 位置設定
  pinMode(SW2,INPUT);//9    I2C 位置設定
  pinMode(SW3,INPUT);//10   I2C 位置設定
  pinMode(SW4,INPUT);//11   I2C 位置設定
  pinMode(MA,OUTPUT);//4    馬達
  pinMode(MB,OUTPUT);//5    馬達
  pinMode(M_PWM,OUTPUT);//6   馬達控速
  Serial.begin(9600);  
  attachInterrupt(digitalPinToInterrupt(SENSOR_Pin),Encoder1,FALLING); //中斷 (設定中斷的腳位，連接到Encoder函數，由電位由HIGH->LOW)
}

String inString="";
char inChar;

void loop() 
{     
//  if(Serial.available())
//  {                   
//      while(Serial.available() > 0)
//      {          
//          inChar = Serial.read();
//          if (isDigit(inChar)) 
//          {  //isDigit 檢查是否是數字(字元0~9)
//            inString += inChar;
//          }
//          if(inChar=='R' || inChar=='r')
//          {
//            CC(255);  // 正轉(全速)            
//            timmer=micros();
//          }
//          
//          if(inChar=='B' || inChar=='b')
//          {
//            CCW(255); //反轉(全速)
//            timmer=micros();
//          }
//          if(inChar=='C' || inChar=='c')
//          {
//            stop1();
//            Serial.println(micros()-timmer);
//            Serial.println(Atest);             
//          }        
//      }//while(Serial.available() > 0)             
//  }//if(Serial.available())
//  if(Atest > 11000)
//  {
//    Afinal=Afinal+1;
//    Atest=0;
//  }
//  //if(Atest >= 11000)
//  if(Afinal >= 5)
//  {
//      stop1();
//      Serial.println(micros()-timmer);
//      Serial.println(Afinal);
//      Serial.println(Atest);
//      Serial.println(Atest2);   
//      while(1);
//  }
//    digitalWrite(MA,LOW);
//    digitalWrite(MB,HIGH);
//    delay(2000);
    digitalWrite(MB,LOW);
    digitalWrite(MA,HIGH);
    delay(2000);
}//void loop() 

void CC(byte S){  //全速正轉
  //byte -->  A byte stores an 8-bit unsigned number, from 0 to 255.
  digitalWrite(MA,LOW); //輸入TA7291P 5腳位 低電位 0V  PD4
  digitalWrite(MB,LOW); //輸入TA7291P 6腳位 低電位 0V  PD5
  analogWrite(M_PWM,S); //(6,255)  輸入TA7291P 4腳位 控制電壓 100%
  delay(500);
  digitalWrite(MA,HIGH); //輸入TA7291P 5腳位 高電位 5V
  digitalWrite(MB,LOW);
  
}

void CCW(byte S){ //全速反轉
  digitalWrite(MA,LOW);
  digitalWrite(MB,LOW);
  analogWrite(M_PWM,S);
  delay(500);
  digitalWrite(MB,HIGH);  //輸入TA7291P 6腳位 高電位 5V
  digitalWrite(MA,LOW);
  
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
  
}



