#include <Wire.h>
const int SLAVE_ADDRESS = 2;
char incomingByte = 0;
void setup()
{ 
    Wire.begin(SLAVE_ADDRESS);    // Slave 一樣是使用 Wire.begin() 加入 I2C bus，
                                  //但是必須傳入一個參數指定所用的 address
    Wire.onReceive(test); // 利用 Wire.onReceive(receiveEvent) 註冊事件，
                                  //之後當 Master 送資料給 Slave 時，
                                  //Arduino 就會自動呼叫 receiveEvent()
    Serial.begin(9600);
    Serial.println("Received data:");
}

void loop() 
{

}
/*void receiveEvent(int howMany)//receiveEvent跟howMany可以隨意更動
{
    //在 receiveEvent() 中，程式的邏輯很簡單，
    //只是利用 Wire.available() 檢查是否有資料，
    //接著利用 Wire.read() 將資料出來再丟到 serial port 上。
    while (Wire.available())
    {
      incomingByte = Wire.read();
      Serial.print(incomingByte);
    }
}*/
void test(int t)//receiveEvent跟howMany可以隨意更動
{
	//Serial.println("receive get");
    //在 receiveEvent() 中，程式的邏輯很簡單，
    //只是利用 Wire.available() 檢查是否有資料，
    //接著利用 Wire.read() 將資料出來再丟到 serial port 上。
    while (Wire.available())
    {
      incomingByte = Wire.read();
      Serial.print(incomingByte);
    }
}

