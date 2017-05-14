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

	{"device",            required_argument,    0,  3},
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
	x.red = 0xFF; \
	x.green = 0x00; \
	x.blue = 0x00;

int options_parse(int argc, char **argv,
	struct option_flags *flags, int8_t *device_number,
	struct option_parse_return *settings)
{
	int c, returnCode = 0;

	INIT_WARNING_LED(settings->warning_led);
	settings->warning_led_temp = 60;

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
			verbose++;
			break;
		case 3:
			sscanf(optarg, "%hhd", device_number);
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
			sscanf(optarg, "%02hhX%02hhX%02hhX", &settings->led_color.red, &settings->led_color.green, &settings->led_color.blue);
			break;
		case 8: /* led warning color */
			sscanf(optarg, "%02hhX%02hhX%02hhX", &settings->warning_led.red, &settings->warning_led.green, &settings->warning_led.blue);
			break;
		case 9: /* led warning temperature */
			sscanf(optarg, "%hhd", &settings->warning_led_temp);
			break;

		case 10:
			sscanf(optarg, "%hhd,%hhd,%hhd,%hhd,%hhd,%hhd",
				&settings->fan1.t1,
				&settings->fan1.t2,
				&settings->fan1.t3,
				&settings->fan1.t4,
				&settings->fan1.t5,
				&settings->fan1.t6);
			break;
		case 11:
			sscanf(optarg, "%hhd,%hhd,%hhd,%hhd,%hhd,%hhd",
				&settings->fan1.s1,
				&settings->fan1.s2,
				&settings->fan1.s3,
				&settings->fan1.s4,
				&settings->fan1.s5,
				&settings->fan1.s6);
			break;
		case 12:
			sscanf(optarg, "%hhu", &settings->pump_mode);
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
	msg_info("\t--device <Device Number> :Select device.\n");

	msg_info("\tLED:\n");
	msg_info("\t\t--led <HTML Color Code> :Define Color for LED.\n");
	msg_info("\t\t--led-warn <HTML Color Code> :Define Color for Warning Temp.\n");
	msg_info("\t\t--led-temp <Temperature in Celsius> :Define Warning Temperature.\n");

	msg_info("\tFan:\n");
	msg_info("\t\t--fan-temps <CSV of Temperatures> :Define Comma Separated Values of Temperatures for Fan.\n");
	msg_info("\t\t--fan-speeds <CSV of Speed Percentage> :Define Comma Separated Values of RPM for Fan.\n");

	msg_info("\tPump:\n");
	msg_info("\t\t--pump <Pump Speed in RPM> :Define RPM Speed of Pump.\n");
}
