/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017  Sean Nelson <audiohacked@gmail.com>

 * OpenCorsairLink is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * any later version.

 * OpenCorsairLink is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with OpenCorsairLink.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _PROTOCOL_ASETEK_H
#define _PROTOCOL_ASETEK_H

#include <stdint.h>
#include "../../common.h"

enum Asetek_Commands {
	ChangeLed = 0x10,
	FanCurve = 0x11,
	UnknownCmd = 0x12,
	PumpMode = 0x13,
};

enum Asetek_FanCurve {
	UnknownFanCurve = 0x00,
};

enum Asetek_PumpModes {
	Asetek_Quiet = 0x28,
	Asetek_Performance = 0x42,
};

#define ASETEK_FAN_TABLE_QUIET(x) struct fan_table x = {\
	.t1 = 0x14, .t2 = 0x20, .t3 = 0x28, .t4 = 0x32, .t5 = 0x37, .t6 = 0x3c,\
	.s1 = 0x19, .s2 = 0x27, .s3 = 0x32, .s4 = 0x4b, .s5 = 0x5a, .s6 = 0x64,\
}

#define ASETEK_FAN_TABLE_EXTREME(x) struct fan_table x = {\
	.t1 = 0x14, .t2 = 0x19, .t3 = 0x1e, .t4 = 0x24, .t5 = 0x30, .t6 = 0x3c,\
	.s1 = 0x32, .s2 = 0x32, .s3 = 0x4b, .s4 = 0x55, .s5 = 0x5d, .s6 = 0x64,\
}

int corsairlink_asetek_device_id(struct corsair_device_info *dev);
int corsairlink_asetek_change_led(struct corsair_device_info *dev,
			struct color *color_led, struct color *warning_led,
			uint8_t Warning_Temp, uint8_t Warning_Enable);
int corsairlink_asetek_pump_mode(struct corsair_device_info *dev,
			uint8_t pump_mode);
int corsairlink_asetek_fan_mode(struct corsair_device_info *dev,
			uint8_t fan_mode);
int corsairlink_asetek_fan_curve(struct corsair_device_info *dev,
			struct fan_table *fan);

#endif 
