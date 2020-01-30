#ifndef _GS_LED_H
#define _GS_LED_H
#include "gs-config.h"

#define FASTLED_INTERNAL
#include <FastLED.h>

#define LED_COUNT 16

extern CRGB led[LED_COUNT];

void gsled_init();
void gsled_commit();

#ifdef LED_SERIAL_ENABLE
void led_com_tasklet();
#endif

#endif
