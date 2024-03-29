#ifndef _GS_CONFIG_H
#define _GS_CONFIG_H

#include <Arduino.h>

/// Game Mode
// Uncomment exactly one mode below
#define MODE_SEGATOOLS
// #define MODE_SEAURCHIN
// #define MODE_OSU4K

/// IR Sensor wiring
#define IR_PIN_START	A0
#define IR_PIN_END		A5
#define IR_ACTIVATION	20
// #define IR_MODE_ANALOG
#define IR_MODE_ACTIVEHIGH
// #define IR_MODE_ACTIVELOW

/// CAP1188 sensor wiring
// #define KB_MODE_CAP1188
#define CL_PIN_CS	10
#define CL_PIN_RST	12
#define CR_PIN_CS	11
#define CR_PIN_RST	-1

/// MPR121 sensor wiring
#define KB_MODE_MPR121
#define CL_ADDR 0x5b
#define CR_ADDR 0x5a

/// LED wiring
#define LED_PIN 8
#define LED_SERIAL_ENABLE

/// Calibrate Button
#define CALIBRATE_PIN 7

#endif
