#include <Wire.h>
//#include <LCD.h>
#include <LiquidCrystal_I2C.h>  

//#define I2C_ADDR    0x3F
//#define BACKLIGHT_PIN  3
//#define En_pin  2
//#define Rw_pin  1
//#define Rs_pin  0
//#define D4_pin  4
//#define D5_pin  5
//#define D6_pin  6
//#define D7_pin  7
//#define BACKLIGHT_FLAG  POSITIVE
//#define LED_OFF  0
//#define LED_ON  1
//LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

#define SENSOR_Pin  2 //Interrupt PIN only D2/D3
#define SENSOR2_Pin 3 //Interrupt PIN only D2/D3
#define MA       4   //馬達
#define MB       5
#define M_PWM    6   // 馬達控速
#define SW1      8    // I2C 位置設定
#define SW2      9    // I2C 位置設定
#define SW3      10   // I2C 位置設定
#define SW4      11   // I2C 位置設定
#define LED      A0
#define M0_VR    A1   // VR 訊號腳位
#define SPD_LOW  90  //速度PWM值下限

int MOVR_VAL,ENC_VAL;
bool working=false;
byte ADDR;
int  SET_VR=500;
int  SPD_VAL;
void setup()
{
  pinMode(SENSOR_Pin,INPUT);//2     Interrupt PIN only D2/D3  外部中斷腳位  只有2跟3
  pinMode(SENSOR2_Pin,INPUT);//3    Interrupt PIN only D2/D3
  pinMode(M0_VR,INPUT);//A1   VR 訊號腳位
  pinMode(SW1,INPUT);//8    I2C 位置設定
  pinMode(SW2,INPUT);//9    I2C 位置設定
  pinMode(SW3,INPUT);//10   I2C 位置設定
  pinMode(SW4,INPUT);//11   I2C 位置設定
  pinMode(LED,OUTPUT);//A0
  pinMode(MA,OUTPUT);//4    馬達
  pinMode(MB,OUTPUT);//5    馬達
  pinMode(M_PWM,OUTPUT);//6   馬達控速
  
  digitalWrite(SENSOR_Pin,LOW);
  digitalWrite(SW1,HIGH);
  digitalWrite(SW2,HIGH);
  digitalWrite(SW3,HIGH);
  digitalWrite(SW4,HIGH);
  digitalWrite(LED,LOW);
  digitalWrite(MA,LOW);
  digitalWrite(MB,LOW);
  analogWrite(M_PWM,255);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(SENSOR_Pin),Encoder,FALLING); //中斷 (設定中斷的腳位，連接到Encoder函數，由電位由HIGH->LOW)
  /*LOW --> to trigger the interrupt whenever the pin is low,
  CHANGE --> to trigger the interrupt whenever the pin changes value
  RISING --> to trigger when the pin goes from low to high,  
  FALLING --> for when the pin goes from high to low.
  The Due, Zero and MKR1000 boards allows also:  
  HIGH --> to trigger the interrupt whenever the pin is high.*/
  ADDR=!digitalRead(SW1)+(!digitalRead(SW2)*2)+(!digitalRead(SW3)*4)+(!digitalRead(SW4)*8);
  Serial.print("Address:");
  Serial.println(ADDR);
  
//  lcd.begin (16,2);//LCD設定 16列 2行
//  lcd.setBacklightPin(BACKLIGHT_PIN,BACKLIGHT_FLAG);
//  lcd.setBacklight(LED_ON);
//  lcd.clear();//將畫面清除，回到左上角
//  delay(100);
//  lcd.home();//回到LCD左上角
//  lcd.backlight();//點亮背光
//  lcd.print("POS:");
  Serial.println("POS:");
//  lcd.setCursor(10,0);//LCD設定第10列第0行
//  lcd.print("V:---");
  Serial.println("V:---");
//  lcd.setCursor(0,1);//第0列第1行
//  lcd.print("ENCODER:");
  Serial.println("ENCODER");
}

//bool A=true;
//bool B=false;

void loop()
{           //SET_VR 預設500
    delay(3000);
//    if(A){
//      A=false;
//      B=true;
//      SET_VR = 900;
//    }
//    else{
//      B=false;
//      A=true;
//      SET_VR = 100;
//    }
//    lcd.setCursor(12,0);
//    lcd.print(SET_VR);
//    lcd.print("  ");//一開始進入LOOP A預設是true LCD顯示V=900，進入後A=flase B=true
    Serial.print("SET_VR= ");
    Serial.println(SET_VR);
    working=true;
    ENC_VAL=0;
  
    if(Serial.available() > 0) 
    {
      String inString = "";
      char inChar;
      while (Serial.available()>0) 
      {
        delay(2);
        inChar = Serial.read();
        
        if(inChar=='R'){
          CC(255);  // 正轉(全速)
        }
        
        if(inChar=='B'){
          CCW(255); //反轉(全速)
        }
      
        if (isDigit(inChar)) {  //isDigit 檢查是否是數字(字元0~9)
          inString += inChar;
        }
      }
      SET_VR = inString.toInt(); //轉換成 long
//      lcd.setCursor(12,0);
//      lcd.print(SET_VR);
//      lcd.print("  ");
      Serial.print("SET_VR = ");
      Serial.println(SET_VR);
      working=true;
    }
    
    GET_VR();
  
//    if(working){  //LOOP一開始 working設定為true
//      bool Going=false;
//      while(MOVR_VAL-SET_VR>=1 || SET_VR-MOVR_VAL>=1){   //可變電阻值-輸入的數字 或  輸入的數字-可變電阻值
//        if(MOVR_VAL>SET_VR){ 
//          SPD_VAL=MOVR_VAL-SET_VR;
//          if(SPD_VAL>255)
//            SPD_VAL=255;
//  
//          if(SPD_VAL<SPD_LOW)
//            SPD_VAL=SPD_LOW;
//          
//          analogWrite(M_PWM,SPD_VAL);
//          if(!Going){
//            Going=true;
//            digitalWrite(MA,HIGH);  //MOVR_VAL-
//            digitalWrite(MB,LOW);
//          } 
//        }
//  
//        if(MOVR_VAL<SET_VR){
//          SPD_VAL=SET_VR-MOVR_VAL;
//          if(SPD_VAL>255)
//            SPD_VAL=255;
//  
//          if(SPD_VAL<SPD_LOW)
//            SPD_VAL=SPD_LOW;
//          
//          analogWrite(M_PWM,SPD_VAL);
//          if(!Going){
//            Going=true;
//            digitalWrite(MB,HIGH);  //MOVR_VAL+
//            digitalWrite(MA,LOW);
//          }
//        }
//        GET_VR();
//      }
//      working=false;
//      MO_BREAK();
////      lcd.setCursor(8,1);
////      lcd.print(ENC_VAL);
////      lcd.print("    ");
//      delay(300);
//    }
}

void GET_VR(){
//  MOVR_VAL=analogRead(M0_VR); //MOVR_VAL == M0_VR
MOVR_VAL = 2000;
//  lcd.setCursor(4,0);
//  lcd.print(MOVR_VAL);
//  lcd.print("  ");
  //lcd.setCursor(8,1);
  //lcd.print(ENC_VAL);
  //lcd.print("    ");
}

void CC(byte S){  //全速正轉
  //byte -->  A byte stores an 8-bit unsigned number, from 0 to 255.
  digitalWrite(MA,LOW); //輸入TA7291P 5腳位 低電位 0V
  digitalWrite(MB,LOW); //輸入TA7291P 6腳位 低電位 0V
  analogWrite(M_PWM,S); //(6,255)  輸入TA7291P 4腳位 控制電壓 100%
  delay(500);
  digitalWrite(MA,HIGH); //輸入TA7291P 5腳位 高電位 5V
  digitalWrite(MB,LOW);
  
}

void CCW(byte S){ //全速反轉
  digitalWrite(MA,LOW);
  digitalWrite(MB,LOW);
  analogWrite(M_PWM,S);
  delay(500);
  digitalWrite(MB,HIGH);  //輸入TA7291P 6腳位 高電位 5V
  digitalWrite(MA,LOW);
}

void MO_BREAK()
{
  digitalWrite(MA,HIGH);
  digitalWrite(MB,HIGH);
}

void Encoder()  //中斷次數
{ 
  ENC_VAL+=1;
}
