#pragma once

#include <Wire.h>

#define I2C_CMD_CONST_MOV 1
#define I2C_CMD_ACC_MOV 2
#define I2C_CMD_TO_SW 20
#define I2C_CMD_SET_ORIGIN 30
#define I2C_CMD_SET_POS 31
#define I2C_CMD_SET_ENABLE 39
#define I2C_CMD_STOP 90
#define I2C_CMD_REQ_STATUS 100
#define I2C_CMD_INV_DIR 110
#define I2C_CMD_SLEEP_MODE 111

// 16 位整數
//union int16u {
//	byte bytes[2];
//	int16_t int16;
//	uint16_t uint16;
//};
//
//// 32 位整數
//union int32u {
//	byte bytes[4];
//	int32_t int32;
//	uint32_t uint32;
//};
typedef unionType int32u ;
typedef unionType2 int16u;

namespace scb {

	struct Status
	{
		int8_t status = 0;
		int32_t pos = 0;
	};

	// I2C slave stop
	void stop(uint8_t addr);
	// I2C slave move
	void mov(uint8_t addr, int32u* target_pos, int16u* pps);
	// I2C slave accelerate move
	void mov_acc(uint8_t addr, int32u* target_pos, int16u* pps, int16u* acc);
	// I2C slave zero
	void zero(uint8_t addr, int16u* pps);
	// I2C slave set enable
	void set_enable(uint8_t addr, uint8_t enable);
	// I2C slave sleep mode
	void set_sleep(uint8_t addr, uint8_t sleep_mode);
	// I2C slave set pos
	void set_pos(uint8_t addr, int32u* pos);
	// I2C slave invert direction
	void set_inv(uint8_t addr, uint8_t inv_dir);

	// I2C all slave stop
	void stop();
	// I2C all slave move
	void mov(int32u* target_pos, int16u* pps);
	// I2C all slave accelerate move
	void mov_acc(int32u* target_pos, int16u* pps, int16u* acc);
	// I2C all slave zero
	void zero(int16u* pps);
	// I2C all slave set enable
	void set_enable(uint8_t* enable);
	// I2C all sleep mode
	void set_sleep(uint8_t* sleep_mode);
	// I2C all set pos
	void set_pos(int32u* pos);
	// I2C all invert direction
	void set_inv(uint8_t* inv_dir);

	// I2C get all slave 
	void get_status(uint8_t addr, Status* status_arr);

	// I2C get all slave 
	void get_status(Status* status_arr);

}

inline void scb::stop(uint8_t addr)
{
	Wire.beginTransmission(addr);
	Wire.write(I2C_CMD_STOP);
	Wire.endTransmission();
}

inline void scb::mov(uint8_t addr, int32u * target_pos, int16u * pps)
{
	Wire.beginTransmission(addr);
	Wire.write(I2C_CMD_CONST_MOV);
	Wire.write(target_pos->bytes, 4);
	Wire.write(pps->bytes, 2);
	Wire.endTransmission();
}

inline void scb::mov_acc(uint8_t addr, int32u * target_pos, int16u * pps, int16u * acc)
{
	Wire.beginTransmission(addr);
	Wire.write(I2C_CMD_ACC_MOV);
	Wire.write(target_pos->bytes, 4);
	Wire.write(pps->bytes, 2);
	Wire.write(acc->bytes, 2);
	Wire.endTransmission();
}

inline void scb::zero(uint8_t addr, int16u * pps)
{
	Wire.beginTransmission(addr);
	Wire.write(I2C_CMD_TO_SW);
	Wire.write(pps->bytes, 2);
	Wire.endTransmission();
}

inline void scb::set_enable(uint8_t addr, uint8_t enable)
{
	Wire.beginTransmission(addr);
	Wire.write(I2C_CMD_SET_ENABLE);
	Wire.write(enable);
	Wire.endTransmission();
}

inline void scb::set_sleep(uint8_t addr, uint8_t sleep_mode)
{
	Wire.beginTransmission(addr);
	Wire.write(I2C_CMD_SLEEP_MODE);
	Wire.write(sleep_mode);
	Wire.endTransmission();
}

inline void scb::set_pos(uint8_t addr, int32u * pos)
{
	Wire.beginTransmission(addr);
	Wire.write(I2C_CMD_SET_POS);
	Wire.write(pos->bytes, 4);
	Wire.endTransmission();
}

inline void scb::set_inv(uint8_t addr, uint8_t inv_dir)
{
	Wire.beginTransmission(addr);
	Wire.write(I2C_CMD_INV_DIR);
	Wire.write(inv_dir);
	Wire.endTransmission();
}

inline void scb::stop()
{
	for (uint8_t i = 1; i <= 6; i++) {
		stop(i);
	}
}

inline void scb::mov(int32u * target_pos, int16u * pps)
{
	for (uint8_t i = 0; i < 6; i++) {
		mov(i + 1, &target_pos[i], &pps[i]);
	}
}

inline void scb::mov_acc(int32u * target_pos, int16u * pps, int16u * acc)
{
	for (uint8_t i = 0; i < 6; i++) {
		mov_acc(i + 1, &target_pos[i], &pps[i], &acc[i]);
	}
}

inline void scb::zero(int16u * pps)
{
	for (uint8_t i = 0; i < 6; i++) {
		zero(i + 1, &pps[i]);
	}
}

inline void scb::set_enable(uint8_t * enable)
{
	for (uint8_t i = 0; i < 6; i++) {
		set_enable(i + 1, enable[i]);
	}
}

inline void scb::set_sleep(uint8_t * sleep_mode)
{
	for (uint8_t i = 0; i < 6; i++) {
		set_sleep(i + 1, sleep_mode[i]);
	}
}

inline void scb::set_pos(int32u * pos)
{
	for (uint8_t i = 0; i < 6; i++) {
		set_pos(i + 1, &pos[i]);
	}
}

inline void scb::set_inv(uint8_t * inv_dir)
{
	for (uint8_t i = 0; i < 6; i++) {
		set_inv(i + 1, inv_dir[i]);
	}
}

inline void scb::get_status(uint8_t addr, Status* scb)
{
	int32u pos;
	const uint8_t i = addr - 1;

	Wire.requestFrom(addr, (uint8_t)5);
	uint8_t wire_avail = Wire.available();
	if (wire_avail >= 5) {
		scb->status = Wire.read();
		for (uint8_t j = 0; j < wire_avail - 1; j++)
		{
			pos.bytes[j] = Wire.read();
		}
		scb->pos = pos.int32;
	}
	else {
		scb->status = -1;
		scb->pos = 0;
	}
}

inline void scb::get_status(Status* scbs)
{
	for (uint8_t i = 0; i < 6; i++)
	{
		get_status(i + 1, &(scbs[i]));
	}
}


