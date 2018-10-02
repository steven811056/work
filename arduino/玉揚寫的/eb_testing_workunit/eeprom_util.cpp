#include "eeprom_util.h"

#include "arduino.h"
#include <EEPROM.h>

void eeprom_write_uint8_t(const uint16_t addr, const uint8_t value)
{
	EEPROM.write(addr, value & 0xff);
}

void eeprom_read_uint8_t(const uint16_t addr, uint8_t & outvalue)
{
	outvalue = EEPROM.read(addr);
}

void eeprom_write_uint16_t(const uint16_t addr, const uint16_t value) {
	EEPROM.write(addr, value >> 8);
	EEPROM.write(addr + 1, value & 0xff);
}

void eeprom_read_uint16_t(const uint16_t addr, uint16_t & outvalue) {
	outvalue = EEPROM.read(addr) << 8;
	outvalue |= EEPROM.read(addr + 1);
}

void eeprom_write_diff_8(const uint16_t addr, const uint8_t value)
{
	uint8_t v_read = 0;
	eeprom_read_uint8_t(addr, v_read);
	if (v_read != value) {
		eeprom_write_uint8_t(addr, value);
	}
}

void eeprom_write_diff_16(const uint16_t addr, const uint16_t value)
{
	uint16_t v_read = 0;
	eeprom_read_uint16_t(addr, v_read);
	if (v_read != value) {
		eeprom_write_uint16_t(addr, value);
	}
}

void eeprom_write_arr(const uint16_t addr, const uint8_t * values, const uint16_t size)
{
	for (uint16_t i = 0; i < size; i++)
	{
		eeprom_write_diff_8(addr + i, values[i]);
	}
}

void eeprom_read_arr(const uint16_t addr, uint8_t * values, const uint16_t size)
{
	for (uint16_t i = 0; i < size; i++)
	{
		eeprom_read_uint8_t(addr + i, values[i]);
	}
}

bool eeprom_equals(const uint16_t addr, uint8_t * values, const uint16_t size)
{
	const uint16_t addrMax = min(E2END + 1, addr + size);
	for (uint16_t i = addr; i < addr + size; i++) {
		if (EEPROM.read(i) != values[i]) {
			return false;
		}
	}
	return true;
}

int8_t eeprom_compare(const uint16_t addr, uint8_t * values, const uint16_t size)
{
	const uint16_t addrMax = min(E2END + 1, addr + size);
	for (uint16_t i = addr; i < addr + size; i++) {
		if (EEPROM.read(i) > values[i]) {
			return -1;
		}
		if (EEPROM.read(i) < values[i]) {
			return 1;
		}
	}
	return 0;
}
