#ifndef _GS_CONFIG_H
#define _GS_CONFIG_H

#include <Arduino.h>

/// Game Mode

#define MODE_SEAURCHIN
// #define MODE_OSU4K

/// IR Sensor wiring

#define IR_PIN_START	A1
#define IR_PIN_END		A5
#define IR_ACTIVATION	200

/// CAP sensor wiring

#define CL_PIN_CS	10
#define CL_PIN_RST	12
#define CR_PIN_CS	11
#define CR_PIN_RST	-1

/// LED wiring
#define LED_PIN 6

#endif
