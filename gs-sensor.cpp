#include "gs-sensor.h"

#include <Arduino.h>

#include "gs-config.h"
#include "gs-reporter.h"
#include "gs-led.h"

uint32_t sensors;
int16_t ir_activation[6];

#ifdef KB_MODE_CAP1188
#include <Adafruit_CAP1188.h>
#include "cap1188-helper.h"
Adafruit_CAP1188 capl = Adafruit_CAP1188(CL_PIN_CS, CL_PIN_RST);
Adafruit_CAP1188 capr = Adafruit_CAP1188(CR_PIN_CS, CR_PIN_RST);
#endif

#ifdef KB_MODE_MPR121
#include "Adafruit_MPR121.h" // TODO switch to library
Adafruit_MPR121 capl = Adafruit_MPR121();
Adafruit_MPR121 capr = Adafruit_MPR121();
#endif

void gssensor_init() {
	sensors = 0;

#ifdef KB_MODE_CAP1188
	capl.begin();
	capr.begin();

	cap1188_set_asc(capl, CAP1188_ASC_AVG_2, CAP1188_ASC_SAMPTIME_1280, CAP1188_ASC_CYCTIME_35);
	cap1188_set_sensitivity(capl, 5);
	cap1188_write(capl, CAP1188_INT_EN, 0);
	cap1188_write(capl, CAP1188_CALCFG, CAP1188_BUT_LD_TH | CAP1188_NEG_DELTA_NONE | CAP1188_CALCFG_256_1024);
	cap1188_set_asc(capr, CAP1188_ASC_AVG_2, CAP1188_ASC_SAMPTIME_1280, CAP1188_ASC_CYCTIME_35);
	cap1188_set_sensitivity(capr, 5);
	cap1188_write(capr, CAP1188_INT_EN, 0);
	cap1188_write(capl, CAP1188_CALCFG, CAP1188_BUT_LD_TH | CAP1188_NEG_DELTA_NONE | CAP1188_CALCFG_256_1024);
	for (int i = 0; i < 8; i++) {
		cap1188_set_threshold(capl, i, 72);
		cap1188_set_threshold(capr, i, 72);
	}
#endif

#ifdef KB_MODE_MPR121
	capl.begin(CL_ADDR);
	capr.begin(CR_ADDR);
#endif

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
	sensors |= ((uint32_t)(uint8_t)capl.touched()) << 8;
	sensors |= (uint32_t)(uint8_t)capr.touched();

	// Read IR LEDs
	for (int i = 0; i < 6; i++) {
		int pinval = analogRead(IR_PIN_START + i);
#ifdef IR_MODE_ANALOG
		if (pinval > ir_activation[i])
			sensors |= (((uint32_t) 1)<<(i+16));
#endif
#ifdef IR_MODE_ACTIVEHIGH
		if (pinval > (1024*IR_ACTIVATION/100))
			sensors |= (((uint32_t) 1)<<(i+16));
#endif
#ifdef IR_MODE_ACTIVELOW
		if (pinval < (1024*IR_ACTIVATION/100))
			sensors |= (((uint32_t) 1)<<(i+16));
#endif
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

#ifdef KB_MODE_CAP1188
	cap1188_calibrate(capl, 0xff);
	cap1188_calibrate(capr, 0xff);
#endif

#ifdef KB_MODE_MPR121
	// TODO calibrate MPR121
#endif

	// Serial.print("IR act: ");
	for (int i = 0; i < 6; i++) {
#ifdef IR_MODE_ANALOG
		int32_t pinval = analogRead(IR_PIN_START + i);
		ir_activation[i] = pinval * IR_ACTIVATION / 100;
		// Serial.print(ir_activation[i]);
		// Serial.print("\t");
#else
		ir_activation[i] = 1024 * IR_ACTIVATION / 100;
#endif
	}
	// Serial.print("\n");
	for (int i = 0; i <16; i++)
		led[i] = 0x00ff00;
	gsled_commit();
	delay(500);
}

void ir_calibrate() {
	int i = 16;
	while (i-->1)
		led[i] = 0;
	led[0] = 0x0000ff;


	gsled_commit();

	while (!(capr.touched() & 1)) {
		for (i = 0; i < 6; i++) {
			int32_t pinval = analogRead(IR_PIN_START + i);
			if (pinval > ir_activation[i]) {
				led[15 - (i*2 + 0)] = 0x00ff00;
			} else {
				led[15 - (i*2 + 0)] = 0xff0000;
			}
			Serial.print(pinval);
			Serial.print("\t");
			pinval = 0xff * pinval / ir_activation[i];
			if (pinval > 0xff)
				pinval = 0xff;
			led[15 - (i*2 + 1)] = (pinval << 16) | (pinval << 8);
		}
		Serial.println();
		gsled_commit();
		delay(10);
	}

	for (int i = 0; i <16; i++)
		led[i] = 0x00ff00;
	gsled_commit();
	delay(500);
}
