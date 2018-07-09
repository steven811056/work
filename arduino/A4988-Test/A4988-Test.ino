int dirPin = 5;
int stepperPin = 4;
int C=0;
int delaytime = 289;

void setup()
{
  pinMode(dirPin, OUTPUT);  
  pinMode(stepperPin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{  
  if(Serial.available())
  {
    C=Serial.parseInt();        
  }  
  if(C==1)
  {
    Serial.println("A");
    A();    
    
  }
  if(C==2)
  {
    Serial.println("B");
    B();    
    
  }
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
//  digitalWrite(dirPin,LOW);  
//  delay(50);  
//  for(int i=0;i<55000;i++ )
//  {  
//    digitalWrite(stepperPin, HIGH);  
//    delayMicroseconds(500);  
//    digitalWrite(stepperPin, LOW);  
//    delayMicroseconds(500); 
//  }
//  delay(50);
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
  C=0;
  
//  digitalWrite(dirPin,HIGH);  
//  delay(50);  
//  for(int i=0;i<100;i++ )
//  {  
//    digitalWrite(stepperPin, HIGH);  
//    delayMicroseconds(delaytime);  
//    digitalWrite(stepperPin, LOW);  
//    delayMicroseconds(delaytime); 
//  }  
//  for(int i=0;i<1000;i++)
//  {
//    digitalWrite(stepperPin, HIGH);  
//    delayMicroseconds(delaytime);  
//    digitalWrite(stepperPin, LOW);  
//    delayMicroseconds(delaytime); 
//    delaytime=delaytime-30;
//  }
//  delay(50);
//  C=0;
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
  C=0;
}

