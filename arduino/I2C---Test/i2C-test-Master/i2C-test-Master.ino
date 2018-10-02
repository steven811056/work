#include <Wire.h>
const int SLAVE_ADDRESS = 2;
char incomingByte = 0;
void setup() 
{ 
    Wire.begin();         // Master 使用 Wire.begin() 加入 I2C bus
    Serial.begin(9600);
   
     
}

void loop()
{
    if(Serial.available())
    {
      incomingByte = Serial.read();
      //每當 Master 要送資料給 Slave 的時候，
      //要先呼叫 Wire.beginTransmission() 建立傳輸，
      //緊接著呼叫 Wire.write() 把資料放到 buffer，
      //最後呼叫 Wire.endTransmission() 真正送出資料並結束傳輸。
      Wire.beginTransmission(SLAVE_ADDRESS);
      Wire.write(incomingByte);
      Serial.print(incomingByte);
      Wire.endTransmission();
    }
}
//當 serial port 上有收到資料時，Arduino 會自動執行 serialEvent()。
//在 serialEvent() 函式中，Master 首先會從 serial port 讀取一個 byte 的資料，
//然後再利用底下三行程式將資料透過 I2C 送給 Slave 1。
/*void serialEvent()*//*緩衝區內有可用資料時會被呼叫的 callback 函式。
                    你可以在這個函式內用 Serial.read() 來取得緩衝區內的資料。*/
/*{
    incomingByte = Serial.read();
    //每當 Master 要送資料給 Slave 的時候，
    //要先呼叫 Wire.beginTransmission() 建立傳輸，
    //緊接著呼叫 Wire.write() 把資料放到 buffer，
    //最後呼叫 Wire.endTransmission() 真正送出資料並結束傳輸。
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(incomingByte);
    Serial.print(incomingByte);
    Wire.endTransmission();
}*/

/*void serialEvent()
{
    incomingByte = Serial.read();
    //每當 Master 要送資料給 Slave 的時候，
    //要先呼叫 Wire.beginTransmission() 建立傳輸，
    //緊接著呼叫 Wire.write() 把資料放到 buffer，
    //最後呼叫 Wire.endTransmission() 真正送出資料並結束傳輸。
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(incomingByte);
    Serial.print(incomingByte);
    Wire.endTransmission();
}*/

