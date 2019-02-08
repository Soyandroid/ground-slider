#include "gs-sensor.h"

#include <Arduino.h>
#include <Adafruit_CAP1188.h>

#include "gs-config.h"
#include "cap1188-helper.h"
#include "gs-reporter.h"
#include "gs-led.h"

uint32_t sensors;

Adafruit_CAP1188 capl = Adafruit_CAP1188(CL_PIN_CS, CL_PIN_RST);
Adafruit_CAP1188 capr = Adafruit_CAP1188(CR_PIN_CS, CR_PIN_RST);

void gssensor_init() {
	sensors = 0;

	capl.begin();
	capr.begin();

	cap1188_set_asc(capl, CAP1188_ASC_AVG_4, CAP1188_ASC_SAMPTIME_640, CAP1188_ASC_CYCTIME_35);
	cap1188_set_sensitivity(capl, 7);
	cap1188_set_asc(capr, CAP1188_ASC_AVG_4, CAP1188_ASC_SAMPTIME_640, CAP1188_ASC_CYCTIME_35);
	cap1188_set_sensitivity(capr, 7);
}

void gssensor_tasklet() {
	uint32_t sensors_prev = sensors;

	sensors = 0;

	// Read capacitive sensors
	sensors |= capl.touched() << 8;
	sensors |= capr.touched();

	// Read IR LEDs
	uint32_t mask = 0x00010000;
	for (int irpin = IR_PIN_START; irpin <= IR_PIN_END; irpin++) {
		if (analogRead(irpin) > IR_ACTIVATION)
			sensors |= mask;
		mask <<= 1;
	}

	gsreporter_update(sensors ^ sensors_prev);
}
