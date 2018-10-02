
#include "StepperEEPROM.h"
#include <Wire.h>
#include "ByteBuffer.h"
#include "fast_io.h"
#include "delay_nanos.h"
#include "serial_util.h"

// PINS
#define PIN_DIR  PIN_A0
#define PIN_STEP PIN_A1
#define PIN_EN   PIN_A2
#define PIN_SW   PIN2
#define PIN_BK   PIN3

// I2C
#define I2C_CMD_CONST_MOV 1
#define I2C_CMD_ACC_MOV 2
#define I2C_CMD_TO_SW 20
#define I2C_CMD_SET_ORIGIN 30
#define I2C_CMD_SET_POS 31
#define I2C_CMD_SET_ENABLE 39
#define I2C_CMD_STOP 90
#define I2C_CMD_REQ_STATUS 100
#define I2C_CMD_INV_DIR 110

// I2C 指令
volatile int8_t i2c_cmd = 0;
// I2C 標記，中斷目前動作
volatile bool i2c_new_inter = false;
// I2C 讀取緩衝區
ByteBuffer wireBuf(32);

// 串列讀取字串
String uart_str;

#define STATUS_IDLE 0          // 工作狀態：待命
#define STATUS_OPERATING 1     // 工作狀態：動作中
// 工作狀態
volatile int8_t status = STATUS_IDLE; 

// 步進馬達狀態
struct Stepper
{
	int32_t pos;
};

// 動作
struct StepperAct
{
	uint16_t pps;               // 每秒脈波數
	int32_t pos_target;         // 目標步數
	uint16_t acc;               // 加速度
};

StepperEEPROM eeprom;

Stepper stepper;
StepperAct act;

// 馬達驅動方向反轉
bool inv_dir = false;
// 馬達電源開關
bool enable = true;

// fast io
volatile uint8_t *port_pin_step;
uint8_t mask_pin_step;
volatile uint8_t *port_pin_dir;
uint8_t mask_pin_dir;
volatile uint8_t *port_pin_sw;
uint8_t mask_pin_sw;
volatile uint8_t *port_pin_bk;
uint8_t mask_pin_bk;

#define fastHigh_step pinFastHigh(port_pin_step, mask_pin_step)
#define fastLow_step pinFastLow(port_pin_step, mask_pin_step)
#define fastHigh_dir pinFastHigh(port_pin_dir, mask_pin_dir)
#define fastLow_dir pinFastLow(port_pin_dir, mask_pin_dir)

#define fastRead_sw pinFastRead(port_pin_sw, mask_pin_sw)
#define fastRead_bk pinFastRead(port_pin_bk, mask_pin_bk)

// Wire onReceive
void receiveEvent(int len) {
	if (len > 0) {
		for (; len > 0; len--)
		{
			wireBuf.write((byte)Wire.read());
		}
		
		SERIAL_MSG_EQU_LN("i2cbytes", wireBuf.available())
		i2c_cmd = wireBuf.getByte();
		SERIAL_MSG_VAL_LN("i2c_cmd")
		i2c_new_inter = i2c_cmd < 100;
	}
}

// Wire onRequest
void requestEvent() {
	if (i2c_cmd < 100) {
		ByteBuffer::int32u u32;
		u32.int32 = stepper.pos;
		Wire.write(status);
		Wire.write(u32.bytes, 4);
		return;
	}

	switch (i2c_cmd)
	{
	case I2C_CMD_REQ_STATUS:
		ByteBuffer::int32u u32;
		u32.int32 = stepper.pos;

		Wire.write(status);
		Wire.write(u32.bytes, 4);

		SERIAL_MSG_VAL_LN(status);
		SERIAL_MSG_EQU_LN("pos", stepper.pos)
		break;
	default:
		break;
	}
}

// 韌體重啟
void(*soft_reset) (void) = 0;

void setup() {
	Serial.begin(115200);

	eeprom.init();
	if (!eeprom.check()) {
		eeprom.wipe_to_default();
	}

	inv_dir = eeprom.get_inv_dir();

	pinMode(PIN_DIR, OUTPUT);
	pinMode(PIN_STEP, OUTPUT);
	pinMode(PIN_EN, OUTPUT);

	pinMode(PIN_SW, INPUT);
	pinMode(PIN_BK, INPUT);

	digitalWrite(PIN_EN, LOW);
	digitalFastLow(PIN_STEP);
	digitalFastLow(PIN_DIR);

	// fast io setup
	port_pin_step = portOutputRegister(digitalPinToPort(PIN_STEP));
	mask_pin_step = digitalPinToBitMask(PIN_STEP);
	port_pin_dir = portOutputRegister(digitalPinToPort(PIN_DIR));
	mask_pin_dir = digitalPinToBitMask(PIN_DIR);

	port_pin_sw = portOutputRegister(digitalPinToPort(PIN_SW));
	mask_pin_sw = digitalPinToBitMask(PIN_SW);
	port_pin_bk = portOutputRegister(digitalPinToPort(PIN_BK));
	mask_pin_bk = digitalPinToBitMask(PIN_BK);

	uint8_t i2c_addr = eeprom.get_i2c_addr();
	Wire.begin(i2c_addr);
	Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent);

	Serial.println("mifes-scb-i2c-stepper");
	Serial.print("i2c_addr=");
	Serial.println(i2c_addr);
	Serial.print("inv_dir=");
	Serial.println(inv_dir);
	Serial.println("ready");
}

void loop() {
	if (i2c_cmd != 0) {
		i2c_new_inter = false;
		status = STATUS_OPERATING;
		switch (i2c_cmd)
		{
		case I2C_CMD_CONST_MOV:
			cmd_const_mov();
			break;
		case I2C_CMD_ACC_MOV:
			cmd_acc_mov();
			break;
		case I2C_CMD_TO_SW:
			cmd_to_sw();
			break;
		case I2C_CMD_SET_ORIGIN:
			cmd_set_origin();
			break;
		case I2C_CMD_SET_POS:
			cmd_set_pos();
			break;
		case I2C_CMD_SET_ENABLE:
			cmd_set_enable();
			break;
		case I2C_CMD_STOP:
			cmd_stop();
			break;
		case I2C_CMD_INV_DIR:
			cmd_inv_dir();
			break;
		default:
			break;
		}
		status = STATUS_IDLE;
	}
	else {
		int bufferd = Serial.available();
		if (bufferd) {
			for (int i = 0; i < bufferd; i++) {
				char c = Serial.read();
				uart_str += c;
				if (c == '\n') {

					if (uart_str.startsWith("i2c_addr=")) {
						uint8_t i2c_addr = uart_str.substring(9, 12).toInt();
						eeprom.set_i2c_addr(i2c_addr);
						eeprom.update_checksum();
						delay(100);
						soft_reset();
					}
					else if (uart_str.startsWith("inv_dir=")) {
						inv_dir = uart_str.substring(8, 9).toInt();
						eeprom.set_inv_dir(inv_dir);
						eeprom.update_checksum();

						Serial.print("inv_dir=");
						Serial.println(inv_dir);
					}

					uart_str = "";
					break;
				}
			}
		}
	}
}

void cmd_const_mov() {
	i2c_cmd = 0;
	StepperAct cmd;
	cmd.pos_target = wireBuf.getInt32();
	cmd.pps = wireBuf.getUInt16();
	wireBuf.reset();

	int32_t act_steps = cmd.pos_target - stepper.pos;
	if (act_steps < 0) {
		act_steps = -act_steps;
	}
	int8_t act_dir = cmd.pos_target > stepper.pos > 0 ? 1 : -1;
	uint32_t pulse_cycle = 1000000 / cmd.pps;
	uint32_t low_delay = pulse_cycle - 8;
	uint32_t long_delay = 0;
	if (cmd.pps < 100) {
		long_delay = (low_delay - 9992) / 1000;
		low_delay = 9992;
	}

	digitalWrite(PIN_DIR, cmd.pos_target > stepper.pos ? HIGH : LOW);

	uint32_t act_real_start_time = micros();

	uint8_t sw_status = LOW;
	int8_t zero_dir = inv_dir ? 1 : -1;
	if (low_delay > 10000) {
		uint16_t long_delay = (low_delay - 9992) >> 10;
		low_delay = 9992;

		for (; act_steps > 0; act_steps--) {
			fastHigh_step;                // 0.0625 us
			delayns1000;                  // 1 us
			fastLow_step;                 // 0.0625 us
			stepper.pos += act_dir;       // 0.0625 us * 8
			delay1tick;
			delay(long_delay);
			delayMicroseconds(low_delay); // 1 us

			sw_status = digitalRead(PIN_SW); // 4.75 us
			if (sw_status == HIGH && act_dir == zero_dir) {      // 0.625 us * 1 ~ 2
				break;
			}

			// new i2c command
			if (i2c_new_inter) {          // 0.625us
				break;
			}
		}
	}
	else {
		for (; act_steps > 0; act_steps--) {
			fastHigh_step;                // 0.0625 us
			delayns1000;                  // 1 us
			fastLow_step;                 // 0.0625 us
			stepper.pos += act_dir;       // 0.0625 us * 8
			delay1tick;
			delayMicroseconds(low_delay); // 1 us

			sw_status = digitalRead(PIN_SW); // 4.75 us
			if (sw_status == HIGH && act_dir == zero_dir) {      // 0.625 us
				break;
			}

			// new i2c command
			if (i2c_new_inter) {          // 0.625us
				break;
			}
		}
	}

	uint32_t act_real_end_time = micros();

	SERIAL_MSGD_START();
	SERIAL_MSGD_EQU("start_time", act_real_start_time);
	SERIAL_MSGD_EQU("end_time", act_real_end_time);
	SERIAL_MSGD_EQU("pass_time", act_real_end_time - act_real_start_time);
	SERIAL_MSGD_LN();
	SERIAL_MSGD_START();
	SERIAL_MSGD_EQU("pos", stepper.pos);
	SERIAL_MSGD_LN();
}

void cmd_acc_mov() {
	i2c_cmd = 0;
	StepperAct cmd;
	cmd.pos_target = wireBuf.getInt32();
	cmd.pps = wireBuf.getUInt16();
	cmd.acc = wireBuf.getUInt16();
	wireBuf.reset();

	SERIAL_MSGD_START();
	SERIAL_MSGD_EQU("target", cmd.pos_target);
	SERIAL_MSGD_EQU("pps", cmd.pps);
	SERIAL_MSGD_EQU("acc", cmd.acc);
	SERIAL_MSGD_LN();

	int32_t act_steps = cmd.pos_target - stepper.pos; // 總步數
	if (act_steps < 0) {
		act_steps = -act_steps;
	}
	int8_t act_dir = cmd.pos_target > stepper.pos > 0 ? 1 : -1; // 步進方向

	// 等速運動期間參數
	uint32_t pulse_cycle = 1000000 / cmd.pps; // 步進波週期
	uint32_t low_delay = pulse_cycle - 8;     // 延遲時間 us
	uint32_t long_delay = 0;                  // 延遲時間 ms

	if (low_delay > 10000) {
		// delayMicroseconds() 只能收 16383 以下，超過部分改成用 delay
		long_delay = (low_delay - 9992) / 1000;
		low_delay = 9992;
	}

	int8_t zero_dir = inv_dir ? 1 : -1; // 極限歸零方向

	digitalWrite(PIN_DIR, cmd.pos_target > stepper.pos ? HIGH : LOW);

	uint32_t acc_take_time = ((double)1000000 / (double)cmd.acc) * cmd.pps; // 以 acc 加速至 pps 所需時間 (us)
	uint32_t acc_move_steps = cmd.pps * (acc_take_time / 2000000);          // 加速期間移動距離
	uint32_t const_move_step = act_steps - (2 * acc_move_steps);            // 定速移動距離

	// 全程加減速運作 
	if (acc_move_steps * 2 > act_steps) {
		acc_move_steps = act_steps >> 1;
		const_move_step = act_steps & 0x1;
	}

	SERIAL_MSGD_START();
	SERIAL_MSGD_EQU("act_steps", act_steps);
	SERIAL_MSGD_EQU("acc_take_time", acc_take_time);
	SERIAL_MSGD_EQU("acc_move_steps", acc_move_steps);
	SERIAL_MSGD_EQU("const_move_step", const_move_step);
	SERIAL_MSGD_LN();

	uint32_t act_real_start_time = micros();
	uint32_t act_real_end_time;

	for (uint32_t step = 1; step <= acc_move_steps; step++)
	{
		fastHigh_step;                // 0.0625 us
		delayns1000;                  // 1 us
		fastLow_step;                 // 0.0625 us
		stepper.pos += act_dir;       // 0.0625 us * 8

		uint32_t step2 = 2 * step; // 0.5 us
		uint32_t acc_delay = 1000000 * (sqrt((double)step2 / (double)cmd.acc) - sqrt((double)(step2 - 2) / (double)cmd.acc)); // ?? s

		if (acc_delay > 10000) {
			uint16_t acc_long_delay = (acc_delay - 9992) / 1000;
			acc_delay = 9992;
			delay(acc_long_delay);
		}
		delayMicroseconds(acc_delay); // 1 us

		uint8_t sw_status = digitalRead(PIN_SW); // 4.75 us
		if (sw_status == HIGH && act_dir == zero_dir) {      // 0.625 us * 1 ~ 2
			return;
		}

		// new i2c command
		if (i2c_new_inter) {          // 0.625us
			return;
		}
	}

	act_real_end_time = micros();
	SERIAL_MSGD_START();
	SERIAL_MSGD_EQU("acc_start_time", act_real_start_time);
	SERIAL_MSGD_EQU("acc_end_time", act_real_end_time);
	SERIAL_MSGD_EQU("acc_pass_time", act_real_end_time - act_real_start_time);
	SERIAL_MSGD_LN();
	SERIAL_MSGD_START();
	SERIAL_MSGD_EQU("pos", stepper.pos);
	SERIAL_MSGD_LN();
	act_real_start_time = micros();

	for (uint32_t step = 1; step <= const_move_step; step++) {
		fastHigh_step;                // 0.0625 us
		delayns1000;                  // 1 us
		fastLow_step;                 // 0.0625 us
		stepper.pos += act_dir;       // 0.0625 us * 8
									  //delayns375;                   // 0.375 us
		delay1tick;
		if (long_delay > 0) {
			delay(long_delay);
		}
		delayMicroseconds(low_delay); // 1 us

		uint8_t sw_status = digitalRead(PIN_SW); // 4.75 us
		if (sw_status == HIGH && act_dir == zero_dir) {      // 0.625 us * 1 ~ 2
			return;
		}

		// new i2c command
		if (i2c_new_inter) {          // 0.625us
			return;
		}
	}

	act_real_end_time = micros();
	SERIAL_MSGD_START();
	SERIAL_MSGD_EQU("acc_start_time", act_real_start_time);
	SERIAL_MSGD_EQU("acc_end_time", act_real_end_time);
	SERIAL_MSGD_EQU("acc_pass_time", act_real_end_time - act_real_start_time);
	SERIAL_MSGD_LN();
	SERIAL_MSGD_START();
	SERIAL_MSGD_EQU("pos", stepper.pos);
	SERIAL_MSGD_LN();
	act_real_start_time = micros();

	for (uint32_t step = acc_move_steps; step > 0; step--)
	{
		fastHigh_step;                // 0.0625 us
		delayns1000;                  // 1 us
		fastLow_step;                 // 0.0625 us
		stepper.pos += act_dir;       // 0.0625 us * 8

		uint32_t step2 = 2 * step; // 0.5 us
		uint32_t acc_delay = 1000000 * (sqrt((double)step2 / (double)cmd.acc) - sqrt((double)(step2 - 2) / (double)cmd.acc)); // ?? s

		if (acc_delay > 10000) {
			uint16_t acc_long_delay = (acc_delay - 9992) / 1000;
			acc_delay = 9992;
			delay(acc_long_delay);
		}
		delayMicroseconds(acc_delay); // 1 us

		uint8_t sw_status = digitalRead(PIN_SW); // 4.75 us
		if (sw_status == HIGH && act_dir == zero_dir) {      // 0.625 us * 1 ~ 2
			return;
		}

		// new i2c command
		if (i2c_new_inter) {          // 0.625us
			return;
		}
	}

	act_real_end_time = micros();
	SERIAL_MSGD_START();
	SERIAL_MSGD_EQU("acc_start_time", act_real_start_time);
	SERIAL_MSGD_EQU("acc_end_time", act_real_end_time);
	SERIAL_MSGD_EQU("acc_pass_time", act_real_end_time - act_real_start_time);
	SERIAL_MSGD_LN();
	SERIAL_MSGD_START();
	SERIAL_MSGD_EQU("pos", stepper.pos);
	SERIAL_MSGD_LN();
}

void cmd_to_sw() {
	i2c_cmd = 0;
	uint16_t pps = wireBuf.getUInt16();
	wireBuf.reset();

	uint16_t pulse_cycle = 1000000 / pps;
	uint16_t low_delay = pulse_cycle - 8;

	digitalWrite(PIN_DIR, inv_dir ? HIGH : LOW);

	uint8_t sw_status = LOW;
	for (; sw_status == LOW; ) {
		fastHigh_step;
		delayns1000;
		fastLow_step;
		delayns1000;
		delayns375;
		delayMicroseconds(low_delay);

		if (i2c_new_inter) {
			break;
		}

		sw_status = digitalRead(PIN_SW);
	}

	stepper.pos = 0;
}

void cmd_set_origin() {
	i2c_cmd = 0;
	wireBuf.reset();

	stepper.pos = 0;

	SERIAL_MSGD_EQU_LN("pos", stepper.pos);
}

void cmd_set_pos() {
	i2c_cmd = 0;
	uint32_t pos = wireBuf.getInt32();
	wireBuf.reset();

	stepper.pos = pos;

	SERIAL_MSGD_EQU_LN("pos", stepper.pos);
}

void cmd_set_enable() {

	i2c_cmd = 0;
	enable = (wireBuf.getUInt8() > 0);
	wireBuf.reset();

	digitalWrite(PIN_EN, enable ? LOW : HIGH);

	SERIAL_MSGD_EQU_LN("enable", enable);
}

void cmd_stop() {
	i2c_cmd = 0;
	wireBuf.reset();

	SERIAL_MSGD_LN("stopped");
}

void cmd_inv_dir() {
	i2c_cmd = 0;
	wireBuf.reset();
	inv_dir = !inv_dir;

	SERIAL_MSGD_VAL_LN(inv_dir);

	eeprom.set_inv_dir(inv_dir);
	eeprom.update_checksum();
}