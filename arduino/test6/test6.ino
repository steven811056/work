#include <Mifes.h>
#include <ArduinoJson.h>

int8_t json_handler(JsonObject& obj) {
    // 在此使用 JsonObject
    String func = obj["func"];

    if(func.equals("hello")){
        StaticJsonBuffer<128> jsonBuffer;
        JsonObject& res = jsonBuffer.createObject();
        res["func"] = func;
        res["res"] = "world";
        res.printTo(Serial);
        return 0; // 0 代表指定被正常的處理。
    }
    return -1; // -1 代表不認識這個指令，要回傳錯誤。
}

void setup() {
    // 初始化主控版程式庫
    mcb::init();
    // Uart 讀取到完整 JSON 時的呼叫函式
    mcb::uart.set_json_handler(json_handler);
    mcb::net.set_json_handler(json_handler);
}

void loop() {
    // 主控版功能循環
    mcb::loop();
}
