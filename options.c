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

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "options.h"
#include "print.h"

static struct option long_options[] = {
	{"help",              no_argument,          0,  0},
	{"version",           no_argument,          0,  1},
	{"debug",             no_argument,          0,  2},

	{"device",            optional_argument,    0,  3},
	{"firmware",          no_argument,          0,  4},

	{"print-temperature", no_argument,          0,  5},
	{"print-fan-speed",   no_argument,          0,  6},

	{"led",               required_argument,    0,  7},
	{"led-warn",          required_argument, NULL,  8},
	{"led-temp",          required_argument, NULL,  9},

	{"fan-temps",         required_argument, NULL,  10},
	{"fan-speeds",        required_argument, NULL,  11},

	{"pump",              required_argument, NULL,  12},

	{0, 0, 0, 0}
};

void options_print(void);

#define INIT_WARNING_LED(x) \
	x->red = 0xFF; \
	x->green = 0x00; \
	x->blue = 0x00;

int options_parse(int argc, char **argv, struct option_flags *flags, 
	struct color *led, struct color *warning, int8_t *warning_temp,
	struct fan_table *fan, struct fan_table *pump, uint8_t *pump_mode)
{
	int c, returnCode = 0;

	INIT_WARNING_LED(warning);
	*warning_temp = 60;

	while (1) {
		int option_index = 0;

		c = getopt_long (argc, argv, "", long_options, &option_index);

		if (c == -1 || returnCode != 0)
			break;

		switch (c) {
		case 0:
			options_print();
			break;
		case 1: /* program version */
			msg_info("OpenCorsairLink Version: %s", VERSION);
			break;

		case 2:
			flags->debug_messages = 1;
			break;
		case 3:
			// sscanf(optarg, "%u", device_number);
			break;
		
		case 4:
			flags->device_firmware = 1;
			break;

		case 5:
			flags->read_temperature = 1;
			break;
		case 6:
			flags->read_fan_speed = 1;
			break;

		case 7: /* led color */
			sscanf(optarg, "%02hhX%02hhX%02hhX", &led->red, &led->green, &led->blue);
			break;
		case 8: /* led warning color */
			sscanf(optarg, "%02hhX%02hhX%02hhX", &warning->red, &warning->green, &warning->blue);
			break;
		case 9: /* led warning temperature */
			sscanf(optarg, "%hhd", warning_temp);
			break;

		case 10:
			sscanf(optarg, "%hhd,%hhd,%hhd,%hhd,%hhd",
				&fan->t1,
				&fan->t2,
				&fan->t3,
				&fan->t4,
				&fan->t5);
			break;
		case 11:
			sscanf(optarg, "%hhd,%hhd,%hhd,%hhd,%hhd",
				&fan->s1,
				&fan->s2,
				&fan->s3,
				&fan->s4,
				&fan->s5);
			break;
		case 12:
			sscanf(optarg, "%hhu", pump_mode);
			break;

		default:
			options_print();
			exit(1);
			returnCode = 0;
		}
	}
	return returnCode;
}

void options_print() {
	msg_info("OpenCorsairLink [options]\n");
	msg_info("Options:\n");
	msg_info("\t--help :Prints this Message\n");
	msg_info("\t--version :Displays version.\n");
	msg_info("\t--debug :Displays enhanced Debug Messages.\n");

	msg_info("\t-l, --led <HTML Color Code> :Define Color for LED.\n");
	msg_info("\t-l, --led-warn <HTML Color Code> :Define Color for Warning Temp.\n");
	msg_info("\t-l, --led-temp <temp in Celsius> :Define Warning Temperature.\n");
}
