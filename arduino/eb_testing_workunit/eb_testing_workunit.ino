#include <ArduinoJson.hpp>
#include <SysCall.h>
#include <SdFatConfig.h>
#include <SdFat.h>
#include <MinimumSerial.h>
#include <FreeStack.h>
#include <BlockDriver.h>
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <EEPROM.h>

#include "bytes_util.h"
#include "charbuf.h"
#include "clock.h"
#include "debug_config.h"
#include "eeprom_util.h"
#include "ebrickunit.h"

/* 處理json的函式 */
void json_handler(JsonObject& root) {

	uint8_t func = root["func"];
	String id = root["id"];
	int16_t ctrl = root["ctrl"];
	int16_t isq = root["isq"];
	String signal = root["signal"];
	String request = root["request"];
	String setup = root["setup"];
	int16_t angle1 = root["angle1"];
	int16_t angle2 = root["angle2"];
	int16_t angle3 = root["angle3"];
	String io = root["io"];
	double x = root["x"];
	double y = root["y"];
	double z = root["z"];
	double e = root["z"];
	double u = root["u"];
	double v = root["v"];
	double w = root["w"];
	int16_t highspeed = root["highspeed"];
	int16_t lowspeed = root["lowspeed"];

	debug_println_2("func", func);
	debug_println_2("id", id);
	debug_println_2("ctrl", ctrl);
	debug_println_2("isq", isq);
	debug_println_2("signal", signal);
	debug_println_2("request", request);
	debug_println_2("setup", setup);
	debug_println_2("angle1", angle1);
	debug_println_2("angle2", angle2);
	debug_println_2("angle3", angle3);
	debug_println_2("io", io);
	debug_println_2("x", x);
	debug_println_2("y", y);
	debug_println_2("z", z);
	debug_println_2("e", e);
	debug_println_2("u", u);
	debug_println_2("v", v);
	debug_println_2("w", w);
	debug_println_2("highspeed", highspeed);
	debug_println_2("lowspeed", lowspeed);

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& res = jsonBuffer.createObject();

	res["func"] = func;
	res["id"] = id;
	res["ctrl"] = ctrl;
	res["isq"] = isq;
	res["fb"] = 1;

	EthernetClient* conn = EBUnit.connection();
	conn->write(0xAA);
	conn->write(0xAA);

	u16buf u16b;
	uint16_t jsonlen = res.measureLength();
	u16b.ui = jsonlen;

	conn->write(u16b.b, 2);

	//conn->write(res.)
	char jsonstr[256];
	res.printTo(jsonstr, 256);

	conn->write(jsonstr);

	u16b.i = checksum(jsonstr, jsonlen);

	conn->write(u16b.b, 2);
}

void setup()
{
	EEPROM.begin();
	Serial.begin(115200);
	// 啟動工作站
	EBUnit.begin();
	// 設定
	EBUnit.handleJson(json_handler);
}

void loop()
{
	// 更新時鐘時間
	Clock.update();
	// 持續處理工作站工作
	EBUnit.handle();
}

