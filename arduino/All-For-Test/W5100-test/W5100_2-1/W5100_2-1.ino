#include <SPI.h>

#include <Ethernet.h> // SPI.h以及Ethernet.h這兩個是必要的引入項。

//自行定義一個mac address，不過注意不要與網路上的其它PC重複了。

byte mac[] = { 0xF0, 0x7B, 0xCB, 0x4B, 0x7C, 0x9F };

//建立一台server傾聽埠號在80 port

EthernetServer server(80);

void setup() {
  Serial.begin(9600);  
  Serial.print("Start configure…");  
  Ethernet.begin(mac);  //依此Mac address啟動網路  
  IPAddress ip = Ethernet.localIP(); //取得DHCP IP  
  server.begin();  //開始Server運作  
  Serial.print("My IP address: ");  
  Serial.print(ip);

}

void loop() {}
