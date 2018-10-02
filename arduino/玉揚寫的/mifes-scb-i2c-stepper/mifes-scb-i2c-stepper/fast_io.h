#ifndef _FASTIO_h
#define _FASTIO_h

#include "arduino.h"

#define pinFastHigh(portReg, mask) (*portReg |= mask)
#define pinFastLow(portReg, mask) (*portReg &= ~mask)
#define pinFastRead(portReg, mask) ((*portReg & mask) ? HIGH : LOW);

inline void digitalFastHigh(uint8_t pin) {
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *out;
	out = portOutputRegister(port);
	*out |= bit;
}

inline void digitalFastLow(uint8_t pin) {
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *out;
	out = portOutputRegister(port);
	*out &= ~bit;
}

inline int digitalFastRead(uint8_t pin) {
	// Disable pwm check
	//uint8_t timer = digitalPinToTimer(pin);

	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	
	// Disable port check
	// if (port == NOT_A_PIN) return LOW;

	// Disable pwm check
	// If the pin that support PWM output, we need to turn it off
	// before getting a digital reading.
	// if (timer != NOT_ON_TIMER) turnOffPWM(timer);

	if (*portInputRegister(port) & bit) return HIGH;
	return LOW;
}

#endif