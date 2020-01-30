#ifndef _CAP1188_HELPER_H
#define _CAP1188_HELPER_H

#define CAP1188_ASC			0x24
#define CAP1188_SENSCTRL	0x1f
#define CAP1188_INPUTDC		0x10
#define CAP1188_INPUTTH		0x30
#define CAP1188_CALIBRATE	0x26
#define CAP1188_INT_EN		0x27
#define CAP1188_CALCFG		0x2f

#define CAP1188_ASC_AVG_1	0x00
#define CAP1188_ASC_AVG_2	0x10
#define CAP1188_ASC_AVG_4	0x20
#define CAP1188_ASC_AVG_8	0x30
#define CAP1188_ASC_AVG_16	0x40
#define CAP1188_ASC_AVG_32	0x50
#define CAP1188_ASC_AVG_64	0x60
#define CAP1188_ASC_AVG_128	0x70

#define CAP1188_ASC_SAMPTIME_320	0x00
#define CAP1188_ASC_SAMPTIME_640	0x04
#define CAP1188_ASC_SAMPTIME_1280	0x08
#define CAP1188_ASC_SAMPTIME_2560	0x0c

#define CAP1188_ASC_CYCTIME_35	0x00
#define CAP1188_ASC_CYCTIME_70	0x01
#define CAP1188_ASC_CYCTIME_105	0x02
#define CAP1188_ASC_CYCTIME_140	0x03

#define CAP1188_BUT_LD_TH		0x80
#define CAP1188_NO_CLR_INTD		0x40
#define CAP1188_NO_CLR_NEG		0x20
#define CAP1188_NEG_DELTA_8		0x00
#define CAP1188_NEG_DELTA_16	0x08
#define CAP1188_NEG_DELTA_32	0x10
#define CAP1188_NEG_DELTA_NONE	0x18
#define CAP1188_CALCFG_16_16	0x00
#define CAP1188_CALCFG_32_32	0x01
#define CAP1188_CALCFG_64_64	0x02
#define CAP1188_CALCFG_128_128	0x03
#define CAP1188_CALCFG_256_256	0x04
#define CAP1188_CALCFG_256_1024	0x05
#define CAP1188_CALCFG_256_2048	0x06
#define CAP1188_CALCFG_256_4096	0x07

#define cap1188_set_asc(dev, avg, samptime, cyctime) ((dev).writeRegister(CAP1188_ASC, ((avg)|(samptime)|(cyctime))))
#define cap1188_set_sensitivity(dev, sensitivity) ((dev).writeRegister(CAP1188_SENSCTRL, ((sensitivity)<<4) | 0x7))
#define cap1188_get_delta(dev, sensor) ((dev).readRegister(CAP1188_INPUTDC | (sensor)))
#define cap1188_set_threshold(dev, sensor, thres) ((dev).writeRegister(CAP1188_INPUTTH | (sensor), thres))
#define cap1188_calibrate(dev, sensormap) ((dev).writeRegister(CAP1188_CALIBRATE, (sensormap)))
#define cap1188_write(dev, reg, val) ((dev).writeRegister((reg), (val)))

#endif
