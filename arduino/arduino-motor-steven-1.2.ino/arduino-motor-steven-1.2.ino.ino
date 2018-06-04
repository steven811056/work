int servoPin = 7;   // pin attached to servo

void t1(int a);

void setup() {                
  pinMode(servoPin, OUTPUT);
  for(int i=0;i<100;i++)
  t1(800);
  delay(100); 
   
}

void loop() {
  for(int i=0;i<100;i++)
  {
    t1(800);   
  }
  delay(100);
  for(int i=0;i<100;i++)
  t1(1000);
  delay(100);
  for(int i=0;i<100;i++)
  t1(1200);
  delay(100); 
  for(int i=0;i<100;i++)
  t1(1650);
  delay(100); 
//  for(int i=0;i<100;i++)
//  t1(2500);
//  delay(100); 

 
 
}
void t1(int x)
{
  digitalWrite(7,HIGH);
  delayMicroseconds(x);
  digitalWrite(7,LOW);
  delay(20);
}

