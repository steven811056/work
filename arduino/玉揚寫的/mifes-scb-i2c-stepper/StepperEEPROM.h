// StepperEEPROM.h

#ifndef _STEPPEREEPROM_h
#define _STEPPEREEPROM_h

#include <Arduino.h>



class StepperEEPROM
{
 protected:

 public:
	 // ��l�� EEPROM
	void init();
	// �� EEPROM ������M
	int16_t checksum();
	// �ˬd EEPROM ������ƥ��T��
	bool check();
	// �M���üg�J�w�]�Ȧ� EEPROM
	void wipe_to_default();
	// ���o����M��
	uint16_t get_checksum();
	// �x�s����M��
	void update_checksum();


	// ���oI2C��}
	uint8_t get_i2c_addr();
	// �]�wI2C��}
	void set_i2c_addr(uint8_t addr);
	// ���o��V����
	uint8_t get_inv_dir();
	// �]�w��V����
	void set_inv_dir(uint8_t inv_dir);
	// ���o�ίv�Ҧ�
	uint8_t get_sleep_mode();
	// �]�w�ίv�Ҧ�
	void set_sleep_mode(uint8_t inv_dir);
};

#endif

