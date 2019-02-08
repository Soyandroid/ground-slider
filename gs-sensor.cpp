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

	cap1188_set_asc(capl, CAP1188_ASC_AVG_2, CAP1188_ASC_SAMPTIME_1280, CAP1188_ASC_CYCTIME_35);
	cap1188_set_sensitivity(capl, 0);
	cap1188_set_asc(capr, CAP1188_ASC_AVG_2, CAP1188_ASC_SAMPTIME_1280, CAP1188_ASC_CYCTIME_35);
	cap1188_set_sensitivity(capr, 0);
	for (int i = 0; i < 8; i++) {
		cap1188_set_threshold(capl, i, 125);
		cap1188_set_threshold(capr, i, 125);
	}
	// Serial.begin(9600);
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

	/*
	for (int i = 0; i < 8; i++) {
		Serial.print((int8_t) cap1188_get_delta(capl, i));
		Serial.print("\t");
	}
	Serial.println();
	*/
	gsreporter_update(sensors ^ sensors_prev);
}
