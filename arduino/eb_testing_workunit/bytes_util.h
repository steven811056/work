#pragma once
#ifndef _BYTES_UTIL_h
#define _BYTES_UTIL_h

#include "Arduino.h"

/* �M���� */
uint16_t checksum(const void* dataptr, uint16_t len);

#endif