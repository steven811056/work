#include <Wire.h>
#include <SPI.h>

#define SlaveAddress 1

String receiveString="";
char incomingByte = 0;

void setup()
{
	Wire.begin(SlaveAddress);
	Serial.begin(9600);
	Wire.onReceive(receive);
//	Wire.onRequest(requestEvent);
	Serial.println("I2C--Salve1---test");

}

void loop()
{


}

void receive(int t)//receiveEvent跟howMany可以隨意更動
{
	  Serial.println("receive get");
    //在 receiveEvent() 中，程式的邏輯很簡單，
    //只是利用 Wire.available() 檢查是否有資料，
    //接著利用 Wire.read() 將資料出來再丟到 serial port 上。
	  if (Wire.available())
	  {
			while (Wire.available())
			{
				receiveString = receiveString + (char)Wire.read();
			}
	  }
	  if (receiveString == "turn");
	  {
		  Wire.onReceive(degreeturn);
	  }
	
	  Serial.print(receiveString);
	  receiveString = "";
	  Serial.println();
}

void degreeturn(int i)
{


}

void requestEvent()
{


}
