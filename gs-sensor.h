#ifndef GS_SENSOR_H
#define GS_SENSOR_H

#include <stdint.h>

#define CL_MASK 0x000000ff
#define CR_MASK 0x0000ff00
#define IR_MASK 0x001f0000

extern uint32_t sensors;

void gssensor_init();
void gssensor_tasklet();

#endif
