#include <Servo.h>
#include <Wire.h>

Servo myservo; // 建立Servo物件，控制伺服馬達
Servo myservo2;

long timmer=0;
double q=0;
int a=0;
float b,c;

void setup() 
{ 
  Wire.begin(); 
  pinMode(8,OUTPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
 // myservo.attach(8); // 連接數位腳位9，伺服馬達的訊號線
 // myservo.write(90);
  myservo2.attach(7,600,2400);
//  myservo2.attach(7);
  myservo2.write(0);
  Serial.begin(9600);
  delay(2000);
} 

void loop() 
{
    for(int i=150;i>50;i=i-10)
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
//  for(int i=0;i<100;i=i+30)
//      {
//        for(int k=0;k<80;k++)
//        {
//          myservo.write(i);  
//          myservo2.write(i);       
//        }
//        Serial.println(i);
//         delay(2000);
//      }
//    while(1);
//    
//  for(int i = 0; i <= 180; i=i+30){
//    timmer=micros();
//    myservo.write(i); // 使用write，傳入角度，從0度轉到180度
//    myservo2.write(i);
//    delay(200);
//  }
//  q=(micros()-timmer)/1000000.0;
//  Serial.println(q);
//  delay(3000);

//  while(1);
    myservo.write(110);// 使用write，傳入角度，從180度轉到0度    
//    myservo2.write(110);
//    delay(2000);
//    myservo.write(0);// 使用write，傳入角度，從180度轉到0度    
//    myservo2.write(0);
//    delay(2000);

//  for(int i = 180; i >= 0; i=i-20){
//    timmer=micros();
//    myservo.write(i);// 使用write，傳入角度，從180度轉到0度
//    //delay(200);
//    myservo2.write(i);
//    Serial.println(i);
//    delay(300);
//  } 
//  q=(micros()-timmer)/1000000.0;
//  Serial.println(q);
//  delay(3000);

//  for(int k=0;k<5;k++)
//  {
//    myservo.write(0);
//    myservo2.write(0);
//    delay(100);
//  }
//  delay(2000);
//  for(int k=0;k<5;k++)
//  {
//    myservo.write(180);
//    myservo2.write(180);
//    delay(100);
//  }
//  delay(2000);

//  val = analogRead(potpin);           // reads the value of the potentiometer (value between 0 and 1023)  
//  Serial.println(val);
//  val = map(val, 0, 1023, 0, 180); 
//  myservo.write(150);                  // sets the servo position according to the scaled value
//  Serial.println(myservo.read());
//  delay(15); 
   
//  for(int i=0;i<30;i++)
//  t900(); 
//  for(int i=0;i<30;i++)
//  t1500(); 
//  myservo2.write(0);
 
}

void t1(int x)
{
  digitalWrite(8,HIGH);
  delayMicroseconds(x);
  digitalWrite(8,LOW);
  delay(20);
}

void t900()
{
  digitalWrite(8,HIGH);
  delayMicroseconds(900);
  digitalWrite(8,LOW);
  delay(20);
  a++;
 
  }
void t1500()
{
  digitalWrite(8,HIGH);
  delayMicroseconds(1500);
  digitalWrite(8,LOW);
  delay(20);
  a--;
}


