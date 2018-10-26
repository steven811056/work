#include <Mifes.h>
#include <ArduinoJson.h>

String str;

void setup() {
  // put your setup code here, to run once:
  mcb::init();
  mcb::uart.set_json_handler(json_handler_uart);
  Serial.begin(115200);
  Serial1.begin(115200);
  
  Serial2.begin(115200);
  Serial3.begin(115200);
  
  //  Serial2.println("{\"choose\":\"0\",\"X\":11,\"Y\":6,\"Z\":2,\"suck\":1}");
   
   // delay(10000);
  //  Serial2.println("{\"choose\":\"1\",\"X\":-17,\"Y\":7,\"Z\":5,\"suck\":0}");
  // Serial2.println("{\"choose\":\"1\",\"X\":-17,\"Y\":7,\"Z\":2.5,\"suck\":0} ");
  // Serial2.println("{\"choose\":\"1\",\"X\":4,\"Y\":17,\"Z\":5,\"suck\":1}");
 
// Serial2.println("{\"choose\":\"1\",\"X\":20,\"Y\":0,\"Z\":3,\"suck\":1}");
 //  Serial2.println("{\"choose\":\"1\",\"X\":-17,\"Y\":6,\"Z\":5,\"suck\":0}");
 //  Serial2.println("{\"choose\":\"1\",\"X\":20,\"Y\":0,\"Z\":3,\"suck\":1}");
}

void loop() {
  // put your main code here, to run repeatedly:

  
 mcb::loop();
 
}


int8_t json_handler(JsonObject& obj, Print& out) {
    // 在此使用 JsonObject
    String func = obj["func"];

    if (func.equals("1")) {

Serial.println("good");
        StaticJsonBuffer<128> jsonBuffer;
        JsonObject& res = jsonBuffer.createObject();
        res["func"] = func;
        res["res"] = "OK";
        res.printTo(out);

       
        Serial2.println("{\"choose\":\"0\",\"X\":11,\"Y\":6,\"Z\":2,\"suck\":1}");
          Serial.println("1");
         delay(7000);
       //歸零
        
        Serial2.println("{\"choose\":\"1\",\"X\":-17,\"Y\":6.2,\"Z\":5,\"suck\":0}");
          Serial.println("2");
         delay(4000);
        //到料台上方
        Serial2.println("{\"choose\":\"1\",\"X\":-17,\"Y\":6.2,\"Z\":2.5,\"suck\":0} ");
          Serial.println("3");
        delay(1500);
        //下降到料台
        Serial2.println("{\"choose\":\"3\",\"X\":-16,\"Y\":9,\"Z\":3,\"suck\":1}");
          Serial.println("4");
        delay(3000);
        //吸盤吸料
      Serial2.println("{\"choose\":\"1\",\"X\":-17,\"Y\":6.2,\"Z\":5,\"suck\":0}");
         Serial.println("5");
        delay(3000);
        //回料台上方
          Serial2.println("{\"choose\":\"1\",\"X\":4,\"Y\":17,\"Z\":5,\"suck\":1}");
          Serial.println("6");
        delay(4000);
        //到目標點上方
        Serial2.println("{\"choose\":\"3\",\"X\":-16,\"Y\":9,\"Z\":2,\"suck\":2}");
          Serial.println("7");
        delay(2000);
        //轉料
       Serial2.println("{\"choose\":\"1\",\"X\":4,\"Y\":17,\"Z\":2,\"suck\":1}");
        delay(4000);
        //下降至目標點
        Serial2.println("{\"choose\":\"3\",\"X\":-16,\"Y\":9,\"Z\":2,\"suck\":0}");
        delay(2000);
        //放料
        Serial2.println("{\"choose\":\"1\",\"X\":4,\"Y\":17,\"Z\":5,\"suck\":1}");
        delay(3000);
        //回目標點上方
        Serial2.println("{\"choose\":\"1\",\"X\":20,\"Y\":0,\"Z\":3,\"suck\":1}");
        
        //手臂平放
        
        
        
        
        
        
        res["res"] = "finish";
        res.printTo(out);

        
        return 0; // 0 代表指定被正常的處理。
    }else if(func.equals("1")){

      Serial3.println("");
      
      }
    return -1; // -1 代表不認識這個指令，要回傳錯誤。
}

// 處理 UART JSON 訊息
int8_t json_handler_uart(JsonObject& obj) {
    return json_handler(obj, mcb::uart.get_serial());
}

