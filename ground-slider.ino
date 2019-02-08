/**
 *	Top Level
 *
 *	Crt0 entry points
 */

#include <Arduino.h>

#include "gs-sensor.h"
#include "gs-led.h"
#include "gs-reporter.h"

void setup() {
	gssensor_init();
	// gsled_init();
	gsreporter_init();
}

void loop() {
	gssensor_tasklet();
}
