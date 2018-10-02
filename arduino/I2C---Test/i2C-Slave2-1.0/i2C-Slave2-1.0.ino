#include <Wire.h>

#define serialbegin 9600
#define slaveAddress 2

void setup()
{  
  Wire.begin(slaveAddress); 
  Wire.onReceive(test);
  Serial.println();
  Serial.begin(serialbegin);
  Serial.println("I2C--Salve1---test");
  
}

void loop() 
{    

}

char slave;
void test(int test)
{  
  while(Wire.available())
  {
    slave=Wire.read();    
    Serial.println("2--轉向");
  } 
}

