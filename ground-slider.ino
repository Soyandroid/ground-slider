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
	gsled_init();
	gssensor_init();
	gsreporter_init();
}

void loop() {
	gssensor_tasklet();
}
