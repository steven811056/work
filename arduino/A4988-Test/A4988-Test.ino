int dirPin = 3;
int stepperPin = 2;

void setup()
{
  pinMode(dirPin, OUTPUT);  
  pinMode(stepperPin, OUTPUT);
}


void loop()
{
  digitalWrite(dirPin,LOW);  
  delay(50);  
  for(int i=0;i<500;i++ )
  {  
    digitalWrite(stepperPin, HIGH);  
    delayMicroseconds(1000);  
    digitalWrite(stepperPin, LOW);  
    delayMicroseconds(1000); 
  }
  delay(50);
}
