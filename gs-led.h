#ifndef _GS_LED_H
#define _GS_LED_H

#define FASTLED_INTERNAL
#include <FastLED.h>

#define LED_COUNT 30

extern CRGB led[LED_COUNT];

void gsled_init();
void gsled_commit();

#endif
