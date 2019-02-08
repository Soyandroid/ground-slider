#include "gs-config.h"

#ifdef MODE_OSU4K

#include "gs-reporter.h"

#include <Arduino.h>
#include <HID-Project.h>

#include "gs-sensor.h"

const char OSU_KEYS[] = "dfjk";

void gsreporter_init() {
	NKROKeyboard.begin();
}

void gsreporter_update(uint32_t modified) {
	if (!modified) {
		return;
	}

	uint32_t mask = 0x00000400;
	for (int i = 0; i < 4; i ++) {
		mask >>= 1;
		if (modified & mask) {
			if (sensors & mask) {
				NKROKeyboard.add(OSU_KEYS[i]);
			} else {
				NKROKeyboard.remove(OSU_KEYS[i]);
			}
		}
	}

	NKROKeyboard.send();
}

#endif
