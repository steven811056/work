#include "bytes_util.h"

uint16_t checksum(const void* dataptr, uint16_t len) {
	uint32_t acc;
	uint16_t src;
	uint8_t *octetptr;

	acc = 0;
	octetptr = (uint8_t*)dataptr;
	while (len > 1) {
		src = (*octetptr) << 8;
		octetptr++;
		src |= (*octetptr);
		octetptr++;
		acc += src;
		len -= 2;
	}
	if (len > 0) {
		src = (*octetptr) << 8;
		acc += src;
	}

	acc = (acc >> 16) + (acc & 0x0000ffffUL);
	if ((acc & 0xffff0000UL) != 0) {
		acc = (acc >> 16) + (acc & 0x0000ffffUL);
	}

	src = (uint16_t)acc;
	return ~src;
}
