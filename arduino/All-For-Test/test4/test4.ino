#include <Wire.h>
 
#define SLAVE_ADDRESS 0x12
#define SERIAL_BAUD 9600
 
uint8_t dataMode = '0';
boolean modeChanged = false;
 
void setup() {
  Wire.begin(SLAVE_ADDRESS);    // join I2C bus as a slave with address 1
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  
  Serial.begin(SERIAL_BAUD);
  Serial.println("I2C Slave.05 started\n");
}
 
void loop() {
  if (modeChanged) {
    Serial.print("Change data mode to ");
    Serial.println((char) dataMode);
    modeChanged = false;
  }
}
 
void receiveEvent(int count) {
  if (Wire.available()) {
    dataMode = Wire.read();
    modeChanged = true;
    Serial.print("Wire.available =");
    Serial.println(Wire.available());
    while (Wire.available())
    {
      Serial.print("Wire.available =");
      Serial.println(Wire.available());
    }
  }
}
 
void requestEvent()
{
  switch (dataMode) {
    case '0':
      Wire.write("Super169",8);
      break;
    case '1':
      Wire.write("Apple II",8);
      break;
    default:
      Wire.write("Who am I",8);
  }
}
