#include "gs-led.h"

#include <FastLED.h>

#include "gs-config.h"

CRGB led[LED_COUNT];

void gsled_init() {
	FastLED.addLeds<NEOPIXEL, LED_PIN>(led, LED_COUNT);
}

void gsled_commit() {
	FastLED.show();
}
