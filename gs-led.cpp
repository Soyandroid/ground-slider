#include "gs-led.h"

#include <FastLED.h>

#include "gs-config.h"

CRGB led[LED_COUNT];

void gsled_init() {
	for (int i = 0; i < LED_COUNT; i++) {
		led[i] = 0x000000;
	}
	FastLED.addLeds<NEOPIXEL, LED_PIN>(led, LED_COUNT);
}

void gsled_commit() {
	FastLED.show();
}
