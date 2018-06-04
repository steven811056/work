// pinStatus_master.ino
// Refer to the "pinStatus_slave" example for use with this
#include <Wire.h>
//#define SLAVE_ADDRESS 1
#define DATA_SIZE 8
const int SLAVE_ADDRESS = 1;
const int clockWire =1;
const int counterclockWire =2;
int A=0; 
int B=0;
int K;

void setup() {  
  Wire.begin();         // join I2C bus as a Master  
  Serial.begin(9600);
  Serial.println("--END SETUP--");  
}

int Q=0;
void loop() 
{   
    if(K<1)
    {       
        if(B == 0)
        {
            requestToSlave(clockWire);
            Serial.println("SLAVE--正轉");
            B=1;
        }  
        Wire.beginTransmission(SLAVE_ADDRESS);  
        Wire.requestFrom(SLAVE_ADDRESS, 1);
        if (Wire.available())               
        {               
            A=Wire.read();
            if(A==100)
            {    
                Serial.println("Request To slave : ");             
                Serial.print("A="); 
                Serial.println(A);
                A=0;
                K=1;    
            }          
        }
        Wire.endTransmission();      
    }
}

void requestToSlave(int W)
{
  Wire.beginTransmission(SLAVE_ADDRESS);
  Serial.print("request TO Slave :");
  Serial.println(W);
  Wire.write(W);
  delay(10);
  Wire.endTransmission();
  
}
