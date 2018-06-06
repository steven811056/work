int dirPin = 3;
int stepperPin = 2;

void setup()
{
  pinMode(dirPin, OUTPUT);  
  pinMode(stepperPin, OUTPUT);
}


void loop()
{
  digitalWrite(dirPin,HIGH);  
  delay(50);  
  for(int i=0;i<500;i++ )
  {  
    digitalWrite(stepperPin, HIGH);  
    delayMicroseconds(10000);  
    digitalWrite(stepperPin, LOW);  
    delayMicroseconds(10000); 
  }
  delay(500);
}
