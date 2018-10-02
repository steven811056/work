#include <Servo.h>

Servo myservo; // 建立Servo物件，控制伺服馬達
Servo myservo2;

char inChar;

void setup() 
{ 
  myservo.attach(9); // 連接數位腳位9，伺服馬達的訊號線
  myservo2.attach(7);
  Serial.begin(9600);
} 

void loop() 
{
  if(Serial.available()>0)
  {
    inChar = Serial.read();
    if(inChar=='R' || inChar=='r')
    {
      for(int i = 0; i <= 360; i+=1)
      {
        myservo.write(i); // 使用write，傳入角度，從0度轉到180度
        myservo2.write(i);
        delay(20);
      }
    }
  }
  
}

