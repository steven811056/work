#include <Wire.h>

#define SENSOR_Pin  2 //Interrupt PIN only D2/D3
#define SENSOR2_Pin 3 //Interrupt PIN only D2/D3
#define MA       5   //馬達
#define MB       6
//#define SLAVE_ADDRESS 1 
const int SLAVE_ADDRESS=1; 
const int clockWire=1;
const int counterclockWire=2;


int ENC_VAL;
unsigned long timmer,Atest2=0;
int Atest=0,Afinal=0,A=0,B=0;
double q=0;
int control;
void setup() 
{
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(Receive);
  Wire.onRequest(Request);
  pinMode(SENSOR_Pin,INPUT);//2     Interrupt PIN only D2/D3  外部中斷腳位  只有2跟3
  pinMode(SENSOR2_Pin,INPUT);//3    Interrupt PIN only D2/D3
  pinMode(MA,OUTPUT);//5    馬達
  pinMode(MB,OUTPUT);//6    馬達
  digitalWrite(MA,LOW);
  digitalWrite(MB,LOW); 
  Serial.begin(9600);  
  attachInterrupt(digitalPinToInterrupt(SENSOR_Pin),Encoder1,FALLING); //中斷 (設定中斷的腳位，連接到Encoder函數，由電位由HIGH->LOW) 
  Serial.println("Start");
}

void loop() 
{  
  if(B == 1)
  { 
    CW(200);    
    timmer=micros();
    Serial.println("正轉");
    while(B<2)
    { 
      delay(10); 
      if(Atest >= 2000)
      {
        Afinal=Afinal+1;        
        stop1();
        q=micros()-timmer;
        Serial.print(q/1000000.0);      
        Serial.println(Afinal);
        Serial.println(Atest);
        Serial.println(Atest2);
        Atest=0;
        B=2; 
      }     
    }    
    Serial.println("STOP");                  
  }   
 
}
void Receive()
{
  if (Wire.available()) 
  {
    control=Wire.read();
    Serial.println("收到mastet訊號");  
    Serial.println(control);    
    if(control == clockWire)
    {    
      Serial.println("Slave接收正轉");    
      B=1;    
    }
    delay(5);  
  }
  
}

void Request()
{ 
  if(B == 2)
  { 
    Serial.println("B=2  ->  B=0");
    Wire.write(100);
    delay(5);
    B=0;
  }  
}

void CW(byte S){  //全速正轉
  //byte -->  A byte stores an 8-bit unsigned number, from 0 to 255.
  digitalWrite(MA,LOW); //輸入TA7291P 5腳位 低電位 0V  PD4
  digitalWrite(MB,LOW); //輸入TA7291P 6腳位 低電位 0V  PD5  
  delay(500);
  analogWrite(MA,S); //輸入TA7291P 5腳位 高電位 5V
//  digitalWrite(MA,HIGH);
  digitalWrite(MB,LOW);
  Serial.print("PWM5腳位: ");
  Serial.println(S);
  
}

void CCW(byte S){ //全速反轉
  digitalWrite(MA,LOW);
  digitalWrite(MB,LOW);
  delay(500);
  analogWrite(MB,S);
//  digitalWrite(MB,HIGH);  //輸入TA7291P 6腳位 高電位 5V
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
}

