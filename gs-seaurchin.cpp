#include "gs-config.h"

#ifdef MODE_SEAURCHIN

#include "gs-reporter.h"

#include <Arduino.h>
#include <HID-Project.h>

#include "gs-sensor.h"

const char SEAURCHIN_KEYS[] = "azsxdcfvgbhnjmk,";
#define SEAURCHIN_AIRUP	KEY_PAGE_UP
#define SEAURCHIN_AIRDN	KEY_PAGE_DOWN
#define SEAURCHIN_AIR	KEY_HOME
#define SEAURCHIN_AIRAC	KEY_END

void gsreporter_init() {
	NKROKeyboard.begin();
}

void gsreporter_update(uint32_t modified) {
	if (!modified) {
		return;
	}

	uint32_t mask = 0x00010000;
	for (int i = 0; i < 16; i ++) {
		mask >>= 1;
		if (modified & mask) {
			if (sensors & mask) {
				NKROKeyboard.add(SEAURCHIN_KEYS[i]);
			} else {
				NKROKeyboard.remove(SEAURCHIN_KEYS[i]);
			}
		}
	}

	NKROKeyboard.send();
}

#endif
