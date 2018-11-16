#include <Servo.h>
#include <Wire.h>

Servo myservo; // 建立Servo物件，控制伺服馬達
Servo myservo2;

int a=0;
float b,c;

void setup() 
{ 
  Wire.begin(); 
//  myservo2.attach(7,600,2400);
  myservo2.attach(7);
  myservo2.write(180);
  Serial.begin(9600);
  delay(1000);
} 

void loop() 
{
    for(int i=180;i>80;i=i-30)
    {
      Serial.print("角度=");
      Serial.println(i);
      Serial.print("A0讀值---\n");      
      for(int k=0;k<80;k++)
      {
        myservo.write(i);  
        myservo2.write(i);
         b=analogRead(A0);
         c=analogRead(A1);
      }
      Serial.println(b*5.0/1023.0);
      Serial.println(c*5.0/1023.0);
      Serial.println((b-c)*5.0/1023.0);     
       delay(2000);
    }
}

void t1(int x)
{
  digitalWrite(8,HIGH);
  delayMicroseconds(x);
  digitalWrite(8,LOW);
  delay(20);
}


