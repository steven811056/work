#include "StepperEEPROM.h"

#include <EEPROM.h>


// I2C位址
#define ROMADDR_I2C_ADDR (0)
#define DEFAULT_I2C_ADDR (1)
// 轉向反轉
#define ROMADDR_INV_DIR (1)
#define DEFAULT_INV_DIR (0)
// I2C位址
#define ROMADDR_SLEEP_MODE (2)
#define DEFAULT_SLEEP_MODE (0)
// 校驗和位址
#define ROMADDR_CHECKSUM (E2END - 1)

void StepperEEPROM::init()
{
	EEPROM.begin();
}

int16_t StepperEEPROM::checksum()
{
	int16_t sum = 0;
	for (uint16_t i = 0; i < (E2END - 2); i += 2) {
		sum += eeprom_read_word((uint16_t *)i);
	}
	return ~sum;
}

bool StepperEEPROM::check()
{
	int16_t checksum_calc = this->checksum();
	int16_t checksum_stored = this->get_checksum();
	return checksum_calc == checksum_stored;
}

void StepperEEPROM::wipe_to_default()
{
	this->set_i2c_addr(DEFAULT_I2C_ADDR);
	this->set_inv_dir(DEFAULT_INV_DIR);
	this->set_sleep_mode(DEFAULT_SLEEP_MODE);

	this->update_checksum();
}

uint16_t StepperEEPROM::get_checksum()
{
	return eeprom_read_word((uint16_t *)ROMADDR_CHECKSUM);
}

void StepperEEPROM::update_checksum()
{
	eeprom_update_word((uint16_t *)ROMADDR_CHECKSUM, this->checksum());
}

uint8_t StepperEEPROM::get_i2c_addr()
{
	return eeprom_read_byte((uint8_t *)ROMADDR_I2C_ADDR);
}

void StepperEEPROM::set_i2c_addr(uint8_t addr)
{
	eeprom_update_byte((uint8_t *)ROMADDR_I2C_ADDR, addr);
}

uint8_t StepperEEPROM::get_inv_dir()
{
	return eeprom_read_byte((uint8_t *)ROMADDR_INV_DIR);
}

void StepperEEPROM::set_inv_dir(uint8_t inv_dir)
{
	eeprom_update_byte((uint8_t *)ROMADDR_INV_DIR, inv_dir);
}

uint8_t StepperEEPROM::get_sleep_mode()
{
	return eeprom_read_byte((uint8_t *)ROMADDR_SLEEP_MODE);
}

void StepperEEPROM::set_sleep_mode(uint8_t sleep_mode)
{
	eeprom_update_byte((uint8_t *)ROMADDR_SLEEP_MODE, sleep_mode);
}


