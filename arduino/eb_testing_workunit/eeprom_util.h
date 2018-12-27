/*
* EEPROM Utility
*/
#ifndef _RBIS_PCS_CLIENT_ARDUINO_EEPROM_UTIL_H_
#define _RBIS_PCS_CLIENT_ARDUINO_EEPROM_UTIL_H_

#include <Arduino.h>

/**
* write 1 byte.
*
* @param  [in] addr is the eeprom address.
* @param  [in] value is input values for write in.
*/
void eeprom_write_uint8_t(const uint16_t addr, const uint8_t value);


/**
* read 1 bytes.
*
* @param  [in] addr is the eeprom address.
* @param  [out] outvalue is the values at the address.
*/
void eeprom_read_uint8_t(const uint16_t addr, uint8_t & outvalue);

/**
* write 2 bytes.
*
* @param  [in] addr is the eeprom address.
* @param  [in] values is input values for write in.
*/
void eeprom_write_uint16_t(const uint16_t addr, const uint16_t value);

/**
* read 2 bytes.
*
* @param  [in] addr is the eeprom address.
* @param  [out] outvalue is the values at the address.
*/
void eeprom_read_uint16_t(const uint16_t addr, uint16_t & outvalue);

/**
* write 1 byte if the the value in address is different.
* this could help to save the EEPROM erase/write cycle.
*
* @param  [in] addr is the eeprom address.
* @param  [in] value is input values for write in.
*/
void eeprom_write_diff_8(const uint16_t addr, const uint8_t value);

/**
* write 2 byte if the the value in address is different.
* this could help to save the EEPROM erase/write cycle.
*
* @param  [in] addr is the eeprom address.
* @param  [in] value is input values for write in.
*/
void eeprom_write_diff_16(const uint16_t addr, const uint16_t value);

/**
* write bytes array if the the value in address is different.
* this could help to save the EEPROM erase/write cycle.
*
* @param  [in] addr is the eeprom address.
* @param  [in] values is input values array for write in.
* @param  [in] size is input values array size.
*/
void eeprom_write_arr(const uint16_t addr, const uint8_t* values, const uint16_t size);

/**
* write bytes array if the the value in address is different.
* this could help to save the EEPROM erase/write cycle.
*
* @param  [in] addr is the eeprom address.
* @param  [out] values is output values array from eeprom.
* @param  [in] size is input values array size.
*/
void eeprom_read_arr(const uint16_t addr, uint8_t* values, const uint16_t size);


/**
* Check if eeprom stored values is eqauls to input values.
*
* @param  [in] addr is the eeprom address.
* @param  [in] values is input values for compare.
* @param  [in] size input values size.
*
* @retval true if input value = eeprom stored values.
*/
bool eeprom_equals(const uint16_t addr, uint8_t * values, const uint16_t size);

/**
* Compare eeprom stored values with input values.
*
* @param  [in] addr is the eeprom address.
* @param  [in] values is input values for compare.
* @param  [in] size input values size.
*
* @retval 0 input value = eeprom stored values.
* @retval 1 input value > eeprom stored values.
* @retval -1 input value < eeprom stored values.
*/
int8_t eeprom_compare(const uint16_t addr, uint8_t * values, const uint16_t size);


#endif

