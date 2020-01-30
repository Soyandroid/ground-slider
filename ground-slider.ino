/**
 *	Top Level
 *
 *	Crt0 entry points
 */

#include <Arduino.h>

#include "gs-sensor.h"
#include "gs-led.h"
#include "gs-reporter.h"
#include "gs-config.h"

void setup() {
	gsled_init();
	gssensor_init();
	gsreporter_init();
}

void loop() {
	gssensor_tasklet();
#ifdef LED_SERIAL_ENABLE
	led_com_tasklet();
#endif
}
