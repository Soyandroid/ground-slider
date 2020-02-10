#include "gs-led.h"

CRGB led[LED_COUNT];
#ifdef LED_SERIAL_ENABLE
static int serial_count;
static int serial_delay, serial_prevdelay;
static unsigned long serial_timer;
#endif

void gsled_init() {
	for (int i = 0; i < LED_COUNT; i++) {
		led[i] = 0x000000;
	}
	FastLED.addLeds<NEOPIXEL, LED_PIN>(led, LED_COUNT);
#ifdef LED_SERIAL_ENABLE
	Serial.begin(115200);
	Serial.setTimeout(0);
	serial_prevdelay = serial_delay = 1200;
	serial_timer = millis();
	serial_count = 0;
#endif
}

void gsled_commit() {
	FastLED.show();
}

#ifdef LED_SERIAL_ENABLE
void led_com_tasklet() {
	if (Serial.available() >= 3) {
		led[serial_count].r = Serial.read();
		led[serial_count].g = Serial.read();
		led[serial_count].b = Serial.read();
		serial_count++;
		if (serial_count == 16) {
			FastLED.show();
			while(Serial.read() != -1);
			serial_delay = serial_prevdelay;
			Serial.write("A"); // ACK
			serial_count = 0;
		}
	}
	if (--serial_delay == 0) {
		while(Serial.read() != -1);
		Serial.write('A');
		int interval = millis() - serial_timer;
		if (interval > 500) {
			serial_prevdelay = serial_prevdelay * 0.9;
		} else {
			serial_prevdelay = serial_prevdelay * 1.1;
		}
		serial_delay = serial_prevdelay;
		serial_timer += interval;
	}
}
#endif
