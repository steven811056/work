int dirPin = 3;
int stepperPin = 2;
int C=0;
int delaytime = 2500;

void setup()
{
  pinMode(dirPin, OUTPUT);  
  pinMode(stepperPin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{  
//  if(Serial.available())
//  {
//    C=Serial.parseInt();        
//  }  
//  if(C==1)
//  {
//    Serial.println("A");
//    A();    
//    C=0;
//  }
//  if(C==2)
//  {
//    Serial.println("B");
//    B();    
//    C=0;
//  }
//  digitalWrite(dirPin,HIGH);  
//  delay(50);  
//  for(int i=0;i<55000;i++ )
//  {  
//    digitalWrite(stepperPin, HIGH);  
//    delayMicroseconds(500);  
//    digitalWrite(stepperPin, LOW);  
//    delayMicroseconds(500); 
//  }
//  delay(50);
  digitalWrite(dirPin,LOW);  
  delay(50);  
  for(int i=0;i<55000;i++ )
  {  
    digitalWrite(stepperPin, HIGH);  
    delayMicroseconds(500);  
    digitalWrite(stepperPin, LOW);  
    delayMicroseconds(500); 
  }
  delay(50);
}

void A()
{
  digitalWrite(dirPin,HIGH);  
  delay(50);  
  for(int i=0;i<5500;i++ )
  {  
    digitalWrite(stepperPin, HIGH);  
    delayMicroseconds(delaytime);  
    digitalWrite(stepperPin, LOW);  
    delayMicroseconds(delaytime); 
  }
  delay(50);
}

void B()
{
  digitalWrite(dirPin,LOW);  
  delay(50);  
  for(int i=0;i<5500;i++ )
  {  
    digitalWrite(stepperPin, HIGH);  
    delayMicroseconds(delaytime);  
    digitalWrite(stepperPin, LOW);  
    delayMicroseconds(delaytime); 
  }
  delay(50);
}

