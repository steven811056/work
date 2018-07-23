//int dirPin = 3;
//int stepperPin = 2;
int dirPin = 5;
int stepperPin = 4;
int C=0;
int delaytime = 15;

void setup()
{
  pinMode(dirPin, OUTPUT);  
  pinMode(stepperPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("start");
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
  for(int i=0;i<55000;i++ )
  {  
    digitalWrite(stepperPin, HIGH);  
    delayMicroseconds(delaytime);  
    digitalWrite(stepperPin, LOW);  
    delayMicroseconds(delaytime);	
  }
  delay(50);
  C=0;
  
 /* digitalWrite(dirPin,HIGH);  
  delay(50);  
  for(int i=0;i<1000;i++ )
  {  
    digitalWrite(stepperPin, HIGH);  
    delayMicroseconds(600);  
    digitalWrite(stepperPin, LOW);  
    delayMicroseconds(600); 
  }  
  digitalWrite(dirPin, HIGH);
  delay(50);
  for(int i=0;i<6000;i++)
  {
    digitalWrite(stepperPin, HIGH);  
    delayMicroseconds(delaytime);  
    digitalWrite(stepperPin, LOW);  
    delayMicroseconds(delaytime);  
	if (delaytime > 150)
	{
	delaytime = delaytime - 10;
	}
  }
  delay(50);
  C=0;*/
}

void B()
{
  digitalWrite(dirPin,LOW);  
  delay(50);  
  for(int i=0;i<55000;i++ )
  {  
    digitalWrite(stepperPin, HIGH);  
    delayMicroseconds(delaytime);  
    digitalWrite(stepperPin, LOW);  
    delayMicroseconds(delaytime); 
  }
  delay(50);
  C=0;
}

