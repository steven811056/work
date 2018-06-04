char chr;
char data[3];
long MyTime=0;
long MyTime2=0;

void setup()
{
 Serial.begin(115200);
}
void loop()
{  

//  if (Serial.available()) {
//      // 讀取傳入的字元值
//    while ((chr = Serial.read()) != '\n') {
//        // 確認輸入的字元介於'0'和'9'，且索引i小於3（確保僅讀取前三個字）
//      if (chr >= '0' && chr <= '9' && i < 3) {
//        data[i] = chr;
//        i++;
//      }
//    }   
//  }
//  Serial.println(micros());
//  for(int i=0;i<500;i++)
//  {
//    digitalWrite(5, HIGH);
//    delayMicroseconds(i);
//    digitalWrite(5, LOW);
//    delayMicroseconds(i);
//  } // 0.26秒

  MyTime=micros();
  int k=1000;
  for(int i=0;i<20;i++)
  {    
    digitalWrite(5, HIGH);
    delayMicroseconds(k);
    digitalWrite(5, LOW);
    delayMicroseconds(k);
  } // 0.04   2.25
  Serial.println(micros()-MyTime);

//  MyTime=micros();
//  float k=5000;
//  while(k>=0)
//  {    
//    digitalWrite(5, HIGH);
//    delayMicroseconds(k);
//    digitalWrite(5, LOW);
//    delayMicroseconds(k);
//    k=k-1.5;
//  } // 25.21   562.5  k=k-1時
//  Serial.println(micros()-MyTime);

//  MyTime=micros();
//  for(int i=5000;i>=2000;i=i-600)
//  {
//    digitalWrite(5, HIGH);
//    delayMicroseconds(i);
//    digitalWrite(5, LOW);
//    delayMicroseconds(i);
////    Serial.println(i);  //一個print拖住大概400 microseconds 
//  } // 0.042  5圈
//  Serial.println(micros()-MyTime);
  
//  MyTime=micros(); 
//  while(MyTime2 <= pow(10,5))
//  {
//    Serial.print("MyTIme---");
//    Serial.println(MyTime); 
//    MyTime2=micros()-MyTime; 
//    Serial.print("MyTime2---");        
//    Serial.println(MyTime2);
//  }
//  Serial.println("END");
  
}
