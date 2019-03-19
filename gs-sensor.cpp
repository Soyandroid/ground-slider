#include "gs-sensor.h"

#include <Arduino.h>
#include <Adafruit_CAP1188.h>

#include "gs-config.h"
#include "cap1188-helper.h"
#include "gs-reporter.h"
#include "gs-led.h"

uint32_t sensors;
int16_t ir_activation[5];

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

	pinMode(CALIBRATE_PIN, INPUT_PULLUP);

	auto_calibrate();
}

void gssensor_tasklet() {

	if (!digitalRead(CALIBRATE_PIN)) {
		auto_calibrate();
		ir_calibrate();
		gsreporter_init();
		return;
	}

	uint32_t sensors_prev = sensors;

	sensors = 0;

	// Read capacitive sensors
	sensors |= capl.touched() << 8;
	sensors |= capr.touched();

	// Read IR LEDs
	for (int i = 0; i < 5; i++) {
		if (analogRead(IR_PIN_START + i) > ir_activation[i])
			sensors |= (1<<(i+16));
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

void auto_calibrate() {
	for (int i = 0; i <16; i++)
		led[i] = 0xff0000;
	gsled_commit();
	delay(3000);
	cap1188_calibrate(capl, 0xff);
	cap1188_calibrate(capr, 0xff);
	for (int i = 0; i < 5; i++) {
		ir_activation[i] = analogRead(IR_PIN_START + i) * IR_ACTIVATION / 100;
	}
	for (int i = 0; i <16; i++)
		led[i] = 0x00ff00;
	gsled_commit();
	delay(1000);
}

void ir_calibrate() {
	led[0] = 0x0000ff;
	for (int i = 15; i > 0; ) {
		led[i--] = 0xff0000;
		led[i--] = 0x000000;
		led[i--] = 0x000000;
	}
	gsled_commit();

	while (!(capr.touched() & 1)) {
		for (int i = 0; i < 5; i++) {
			long pinval = analogRead(IR_PIN_START + i);
			if (pinval > ir_activation[i]) {
				led[15 - (i*3 + 0)] = 0x00ff00;
			} else {
				led[15 - (i*3 + 0)] = 0xff0000;
			}
			pinval = 0xff * pinval / ir_activation[i];
			if (pinval > 0xff)
				pinval = 0xff;
			led[15 - (i*3 + 1)] = (pinval << 16) | (pinval << 8);
			// Serial.print(pinval);
			// Serial.print("\t");
		}
		// Serial.println();
		gsled_commit();
		delay(10);
	}

	for (int i = 0; i <16; i++)
		led[i] = 0x00ff00;
	gsled_commit();
	delay(1000);
}
