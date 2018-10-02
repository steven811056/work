#pragma once

union u16buf
{
	uint8_t b[2];
	int16_t i;
	uint16_t ui;
};

union u32buf
{
	uint8_t b[4];
	int32_t i;
	uint32_t ui;
};