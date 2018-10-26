
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
#define I2C_CMD_BRAKES 80
#define I2C_CMD_STOP 90
#define I2C_CMD_STOP_ADV 91
#define I2C_CMD_REQ_STATUS 100
#define I2C_CMD_REQ_SETTING 101
#define I2C_CMD_INV_DIR 110
#define I2C_CMD_SLEEP_MODE 111

// I2C ���O
volatile int8_t i2c_cmd = 0;
// I2C �аO�A���_�ثe�ʧ@
volatile bool i2c_new_inter = false;
// I2C Ū���w�İ�
ByteBuffer wireBuf(32);

// ��CŪ���r��
String uart_str;

#define STATUS_IDLE 0          // �u�@���A�G�ݩR
#define STATUS_OPERATING 1     // �u�@���A�G�ʧ@��
// �u�@���A
volatile int8_t status = STATUS_IDLE; 

// �B�i���F���A
struct Stepper
{
	int32_t pos;
};

// �ʧ@
struct StepperAct
{
	int32_t pos_target;         // �ؼШB��
	uint16_t pps;               // �C��ߪi��
	uint16_t acc;               // �[�t��
};

// �x�s��
StepperEEPROM eeprom;
// �B�i���F���A
Stepper stepper;
// ���O�ʧ@
StepperAct act;

// ���F�X�ʤ�V����
// �t�~�A�����k�s��V�����F����A�o�ӼаO�Ω�ץ������k�s��V
bool inv_dir = false;
// ���F�q���}��
bool enable = true;
// ���F�R��������q�y�Ҧ�
bool sleep_mode = false;

// fast io �ֳt�}���X�Ѽ�
volatile uint8_t *port_pin_en;
uint8_t mask_pin_en;
volatile uint8_t *port_pin_step;
uint8_t mask_pin_step;
volatile uint8_t *port_pin_dir;
uint8_t mask_pin_dir;
volatile uint8_t *port_pin_sw;
uint8_t mask_pin_sw;
volatile uint8_t *port_pin_bk;
uint8_t mask_pin_bk;

// �N ENABLE �}�Կ�X ���q��
#define fastHigh_en pinFastHigh(port_pin_en, mask_pin_en)
// �N ENABLE �}�Կ�X �C�q��
#define fastLow_en pinFastLow(port_pin_en, mask_pin_en)
// �N STEP �}�Կ�X ���q��
#define fastHigh_step pinFastHigh(port_pin_step, mask_pin_step)
// �N STEP �}�Կ�X �C�q��
#define fastLow_step pinFastLow(port_pin_step, mask_pin_step)
// �N DIR  �}�Կ�X ���q��
#define fastHigh_dir pinFastHigh(port_pin_dir, mask_pin_dir)
// �N DIR  �}�Կ�X �C�q��
#define fastLow_dir pinFastLow(port_pin_dir, mask_pin_dir)
// �N BK   �}�Կ�X ���q��
#define fastHigh_bk pinFastHigh(port_pin_bk, mask_pin_bk)
// �N BK   �}�Կ�X �C�q��
#define fastLow_bk pinFastLow(port_pin_bk, mask_pin_bk)

// Wire onReceive
void receiveEvent(int len) {
	if (len > 0) {
		for (; len > 0; len--)
		{
			wireBuf.write((byte)Wire.read());
		}
		
		SERIAL_MSG_EQU_LN("i2cbytes", wireBuf.available())
		i2c_cmd = wireBuf.getByte();
		SERIAL_MSG_VAL_LN(i2c_cmd)
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
		break;
	case I2C_CMD_REQ_SETTING:
		Wire.write(enable);
		Wire.write(inv_dir);
		Wire.write(sleep_mode);
		break;
	default:
		break;
	}
}

// ���魫��
void(*soft_reset) (void) = 0;

void setup() {
	Serial.begin(115200);

	eeprom.init();
	if (!eeprom.check()) {
		eeprom.wipe_to_default();
	}

	inv_dir = eeprom.get_inv_dir();
	sleep_mode = eeprom.get_sleep_mode();

	pinMode(PIN_DIR, OUTPUT);
	pinMode(PIN_STEP, OUTPUT);
	pinMode(PIN_EN, OUTPUT);
	pinMode(PIN_BK, OUTPUT);

	pinMode(PIN_SW, INPUT);

	digitalWrite(PIN_EN, (sleep_mode != 0) ? HIGH : LOW);
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
	Serial.print("sleep_mode=");
	Serial.println(sleep_mode);
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
		case I2C_CMD_BRAKES:
			cmd_brakes();
			break;
		case I2C_CMD_STOP:
			cmd_stop();
			break;
		case I2C_CMD_INV_DIR:
			cmd_inv_dir();
			break;
		case I2C_CMD_SLEEP_MODE:
			cmd_sleep_mode();
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
					else if (uart_str.startsWith("sleep_mode=")) {
						sleep_mode = uart_str.substring(11, 12).toInt();
						eeprom.set_sleep_mode(sleep_mode);
						eeprom.update_checksum();

						Serial.print("sleep_mode=");
						Serial.println(sleep_mode);
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

	digitalWrite(PIN_EN, LOW);
	digitalWrite(PIN_DIR, (cmd.pos_target > stepper.pos ? (inv_dir ? LOW : HIGH) : (inv_dir ? HIGH : LOW)));

	uint32_t act_real_start_time = micros();

	uint8_t sw_status = LOW;
	//int8_t zero_dir = inv_dir ? 1 : -1;
	int8_t zero_dir = -1;
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

	digitalWrite(PIN_EN, (sleep_mode != 0) ? HIGH : LOW);
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

	int32_t act_steps = cmd.pos_target - stepper.pos; // �`�B��
	if (act_steps < 0) {
		act_steps = -act_steps;
	}
	int8_t act_dir = cmd.pos_target > stepper.pos > 0 ? 1 : -1; // �B�i��V

	// ���t�B�ʴ����Ѽ�
	uint32_t pulse_cycle = 1000000 / cmd.pps; // �B�i�i�g��
	uint32_t low_delay = pulse_cycle - 8;     // ����ɶ� us
	uint32_t long_delay = 0;                  // ����ɶ� ms

	if (low_delay > 10000) {
		// delayMicroseconds() �u�ব 16383 �H�U�A�W�L�����令�� delay
		long_delay = (low_delay - 9992) / 1000;
		low_delay = 9992;
	}

	//int8_t zero_dir = inv_dir ? 1 : -1; // �����k�s��V
	int8_t zero_dir = -1;

	digitalWrite(PIN_EN, LOW);
	digitalWrite(PIN_DIR, (cmd.pos_target > stepper.pos ? (inv_dir ? LOW : HIGH) : (inv_dir ? HIGH : LOW)));

	uint32_t acc_take_time = ((double)1000000 / (double)cmd.acc) * cmd.pps; // �H acc �[�t�� pps �һݮɶ� (us)
	uint32_t acc_move_steps = cmd.pps * ((double)acc_take_time / (double)2000000); // �[�t�������ʶZ��
	uint32_t const_move_step = act_steps - (2 * acc_move_steps);            // �w�t���ʶZ��

	// ���{�[��t�B�@ 
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

	SERIAL_MSGD_LN("acc start");
	uint32_t act_real_start_time = micros();
	uint32_t act_real_end_time;

	const uint16_t time_offset = 96;

	uint32_t last_step_time = 0;

	for (uint32_t step = 1; step <= acc_move_steps; step++)
	{
		//uint32_t act_step_start_time = micros();
		fastHigh_step;                // 0.0625 us
		delayns1000;                  // 1 us
		fastLow_step;                 // 0.0625 us
		stepper.pos += act_dir;       // 0.0625 us * 8

		//uint32_t act_real_start_time = micros();

		// calc cost 84 ~ 100 us
		// 1000000 * sqrt((2 * s) / a) 
		// = 1000000 * sqrt(2) * sqrt(s / a)
		// = 1414213.562373095 * sqrt(s / a)
		uint32_t this_step_time = 1414213.562373095 * sqrt((double)(step) / (double)cmd.acc);
		uint32_t acc_delay = this_step_time - last_step_time;
		last_step_time = this_step_time;
		//uint32_t act_real_end_time = micros();
		//SERIAL_MSGD_EQU_LN("q", act_real_end_time - act_real_start_time);

		if (acc_delay > 16000) {
			uint16_t acc_long_delay = (acc_delay - 10000) / 1000;
			acc_delay = 10000 - time_offset;
			delay(acc_long_delay);
		}
		if (acc_delay > time_offset) {
			delayMicroseconds(acc_delay - time_offset); // 1 us
		}

		uint8_t sw_status = digitalRead(PIN_SW); // 4.75 us
		if (sw_status == HIGH && act_dir == zero_dir) {      // 0.625 us * 1 ~ 2
			return;
		}

		// new i2c command
		if (i2c_new_inter) {          // 0.625us
			return;
		}
		//uint32_t act_step_end_time = micros();
		//SERIAL_MSGD_START();
		//SERIAL_MSGD_EQU("ss", act_step_start_time);
		//SERIAL_MSGD_EQU("se", act_step_end_time);
		//SERIAL_MSGD_EQU("sp", act_step_end_time - act_step_start_time);
		//SERIAL_MSGD_EQU("sd", acc_delay);
		//SERIAL_MSGD_LN();
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

	SERIAL_MSGD_LN("con start");
	act_real_start_time = micros();

	for (uint32_t step = 1; step <= const_move_step; step++) {
		fastHigh_step;                // 0.0625 us
		delayns1000;                  // 1 us
		fastLow_step;                 // 0.0625 us
		stepper.pos += act_dir;       // 0.0625 us * 8
		delayns1000;                  // 1 us
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
	SERIAL_MSGD_EQU("con_start_time", act_real_start_time);
	SERIAL_MSGD_EQU("con_end_time", act_real_end_time);
	SERIAL_MSGD_EQU("con_pass_time", act_real_end_time - act_real_start_time);
	SERIAL_MSGD_LN();
	SERIAL_MSGD_START();
	SERIAL_MSGD_EQU("pos", stepper.pos);
	SERIAL_MSGD_LN();

	SERIAL_MSGD_LN("dcc start");
	act_real_start_time = micros();

	last_step_time = 1414213.562373095 * sqrt((double)(acc_move_steps) / (double)cmd.acc);

	for (uint32_t step = acc_move_steps; step > 0; step--)
	{
		fastHigh_step;                // 0.0625 us
		delayns1000;                  // 1 us
		fastLow_step;                 // 0.0625 us
		stepper.pos += act_dir;       // 0.0625 us * 8

		//const uint32_t step2 = 2 * step; // 0.5 us
		//uint32_t acc_delay = 1000000 * (sqrt((double)step2 / (double)cmd.acc) - sqrt((double)(step2 - 2) / (double)cmd.acc)); // ?? s

		uint32_t this_step_time = 1414213.562373095 * sqrt((double)(step) / (double)cmd.acc);
		uint32_t acc_delay = last_step_time - this_step_time;
		last_step_time = this_step_time;

		if (acc_delay > 16000) {
			uint16_t acc_long_delay = (acc_delay - 10000) / 1000;
			acc_delay = 10000 - time_offset;
			delay(acc_long_delay);
		}
		if (acc_delay > time_offset) {
			delayMicroseconds(acc_delay - time_offset); // 1 us
		}

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
	SERIAL_MSGD_EQU("dcc_start_time", act_real_start_time);
	SERIAL_MSGD_EQU("dcc_end_time", act_real_end_time);
	SERIAL_MSGD_EQU("dcc_pass_time", act_real_end_time - act_real_start_time);
	SERIAL_MSGD_LN();
	SERIAL_MSGD_START();
	SERIAL_MSGD_EQU("pos", stepper.pos);
	SERIAL_MSGD_LN();

	digitalWrite(PIN_EN, (sleep_mode != 0) ? HIGH : LOW);
}

void cmd_to_sw() {
	i2c_cmd = 0;
	uint16_t pps = wireBuf.getUInt16();
	wireBuf.reset();

	uint16_t pulse_cycle = 1000000 / pps;
	uint16_t low_delay = pulse_cycle - 8;

	digitalWrite(PIN_EN, LOW);
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
	digitalWrite(PIN_EN, (sleep_mode != 0) ? HIGH : LOW);
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

void cmd_brakes() {
	i2c_cmd = 0;
	bool brakes_release;
	if (wireBuf.available()) {
		brakes_release = wireBuf.getByte() != 0;
	}
	else {
		brakes_release = !false;
	}
	wireBuf.reset();

	if (brakes_release) {
		fastHigh_bk;
	}
	else {
		fastLow_bk;
	}

	SERIAL_MSGD_EQU_LN("brakes", brakes_release);
}

void cmd_stop() {
	i2c_cmd = 0;
	wireBuf.reset();

	SERIAL_MSGD_LN("stopped");
}

void cmd_inv_dir() {
	i2c_cmd = 0;
	if (wireBuf.available()) {
		inv_dir = wireBuf.getByte() != 0;
	}
	else {
		inv_dir = !inv_dir;
	}
	wireBuf.reset();

	SERIAL_MSGD_VAL_LN(inv_dir);

	eeprom.set_inv_dir(inv_dir);
	eeprom.update_checksum();
}

void cmd_sleep_mode() {

	i2c_cmd = 0;
	if (wireBuf.available()) {
		sleep_mode = wireBuf.getByte() != 0;
	}
	else {
		sleep_mode = !sleep_mode;
	}
	wireBuf.reset();

	SERIAL_MSGD_VAL_LN(sleep_mode);

	eeprom.set_inv_dir(sleep_mode);
	eeprom.update_checksum();
}