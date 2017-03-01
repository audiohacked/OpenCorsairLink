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

#ifndef _OPTIONS_H
#define _OPTIONS_H

#include <stdint.h>
#include "common.h"


struct color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

struct fan_table {
	/** temperatures */
	int8_t t1;
	int8_t t2;
	int8_t t3;
	int8_t t4;
	int8_t t5;
	int8_t t6;

	/** fan speeds */
	int8_t s1;
	int8_t s2;
	int8_t s3;
	int8_t s4;
	int8_t s5;
	int8_t s6;
};

enum pump_modes {
	PWM = 0,
	RPM = 1,
	DEFAULT = 2,
	QUIET = 3,
	BALANCED = 4,
	PERFORMANCE = 5,
	CUSTOM = 6,
};

struct option_flags {
	// flags
	//unsigned int debug_messages:1;
	unsigned int device_firmware:1;
	unsigned int read_temperature:1;
	unsigned int read_fan_speed:1;
};

struct option_parse_return {
	struct color led_color;
	struct color warning_led;
	int8_t warning_led_temp;
	struct fan_table fan1;
	struct fan_table pump;
	uint8_t pump_mode;
};

int options_parse(int argc, char **argv,
	struct option_flags *flags, int8_t *device_number,
	struct option_parse_return *settings);

#endif
