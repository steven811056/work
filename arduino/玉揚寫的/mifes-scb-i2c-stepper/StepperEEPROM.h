// StepperEEPROM.h

#ifndef _STEPPEREEPROM_h
#define _STEPPEREEPROM_h

#include <Arduino.h>



class StepperEEPROM
{
 protected:

 public:
	 // 初始化 EEPROM
	void init();
	// 對 EEPROM 取校驗和
	int16_t checksum();
	// 檢查 EEPROM 內的資料正確性
	bool check();
	// 清除並寫入預設值至 EEPROM
	void wipe_to_default();
	// 取得校驗和值
	uint16_t get_checksum();
	// 儲存校驗和值
	void update_checksum();


	// 取得I2C位址
	uint8_t get_i2c_addr();
	// 設定I2C位址
	void set_i2c_addr(uint8_t addr);
	// 取得轉向反轉
	uint8_t get_inv_dir();
	// 設定轉向反轉
	void set_inv_dir(uint8_t inv_dir);
	// 取得睡眠模式
	uint8_t get_sleep_mode();
	// 設定睡眠模式
	void set_sleep_mode(uint8_t inv_dir);
};

#endif

