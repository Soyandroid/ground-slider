#include "gs-config.h"

#ifdef MODE_SEGATOOLS

#include "gs-reporter.h"

#include <Arduino.h>
#include <HID-Project.h>

#include "gs-sensor.h"
#include "gs-led.h"

const char SEGATOOLS_KEYS[] = "987654azsxdcfvgbhnjmk,";
#define SEGATOOLS_KEYCOLOR_ON	0xff00ff
#define SEGATOOLS_KEYCOLOR_OFF	0xffff33

void gsreporter_init() {
	NKROKeyboard.begin();

	for (int i = 0; i <16; i++) {
#ifndef LED_SERIAL_ENABLE
		led[i] = SEGATOOLS_KEYCOLOR_OFF;
#else
		led[i] = 0;
#endif
	}
	gsled_commit();
}

void gsreporter_update(uint32_t modified) {
	if (!modified) {
		return;
	}

	uint32_t mask = 0x400000;
	for (int i = 0; i < 22; i++) {
		mask >>= 1;

		if (modified & mask) {
			if (sensors & mask) {
				NKROKeyboard.add(SEGATOOLS_KEYS[i]);
#ifndef LED_SERIAL_ENABLE
				if (i < 16) {
					led[15-i] = SEGATOOLS_KEYCOLOR_ON;
				}
#endif
			} else {
				NKROKeyboard.remove(SEGATOOLS_KEYS[i]);
#ifndef LED_SERIAL_ENABLE
				if (i < 16) {
					led[15-i] = SEGATOOLS_KEYCOLOR_OFF;
				}
#endif
			}
		}
	}

	NKROKeyboard.send();
#ifndef LED_SERIAL_ENABLE
	gsled_commit();
#endif
}

#endif
