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
	{"dump",              no_argument,          0,  3},
	
	{"device",            required_argument,    0,  4},
	{"firmware",          no_argument,          0,  5},

	{"print-temperature", no_argument,          0,  6},
	{"print-fan-speed",   no_argument,          0,  7},

	{"led",               required_argument,    0,  8},
	{"led-warn",          required_argument, NULL,  9},
	{"led-temp",          required_argument, NULL,  10},

	{"fan-temps",         required_argument, NULL,  11},
	{"fan-speeds",        required_argument, NULL,  12},

	{"pump-mode",         required_argument, NULL,  13},

	{0, 0, 0, 0}
};

void options_print(void);

#define INIT_WARNING_LED(xx) \
	xx.red = 0xFF; \
	xx.green = 0x00; \
	xx.blue = 0x00;

int options_parse(int argc, char **argv,
	struct option_flags *flags, int8_t *device_number,
	struct option_parse_return *settings)
{
	int cc, returnCode = 0;

	memset(settings, 0, sizeof(struct option_parse_return));
	INIT_WARNING_LED(settings->warning_led);
	settings->warning_led_temp = 60;
	settings->pump_mode = DEFAULT;

	while (1) {
		int option_index = 0;

		cc = getopt_long (argc, argv, "", long_options, &option_index);

		if (cc == -1 || returnCode != 0)
			break;

		switch (cc) {
		case 0:
			options_print();
			break;
		case 1: /* program version */
			msg_info("OpenCorsairLink Version: %s", VERSION);
			break;
		case 3:
			verbose++;
		case 2:
			verbose++;
			break;
		case 4:
			sscanf(optarg, "%hhd", device_number);
			break;
		
		case 5:
			flags->device_firmware = 1;
			break;

		case 6:
			flags->read_temperature = 1;
			break;
		case 7:
			flags->read_fan_speed = 1;
			break;

		case 8: /* led color */
			sscanf(optarg, "%02hhX%02hhX%02hhX", &settings->led_color.red, &settings->led_color.green, &settings->led_color.blue);
			break;
		case 9: /* led warning color */
			sscanf(optarg, "%02hhX%02hhX%02hhX", &settings->warning_led.red, &settings->warning_led.green, &settings->warning_led.blue);
			break;
		case 10: /* led warning temperature */
			sscanf(optarg, "%hhd", &settings->warning_led_temp);
			break;

		case 11:
			sscanf(optarg, "%hhd,%hhd,%hhd,%hhd,%hhd,%hhd",
				&settings->fan1.t1,
				&settings->fan1.t2,
				&settings->fan1.t3,
				&settings->fan1.t4,
				&settings->fan1.t5,
				&settings->fan1.t6);
			break;
		case 12:
			sscanf(optarg, "%hhd,%hhd,%hhd,%hhd,%hhd,%hhd",
				&settings->fan1.s1,
				&settings->fan1.s2,
				&settings->fan1.s3,
				&settings->fan1.s4,
				&settings->fan1.s5,
				&settings->fan1.s6);
			break;
		case 13:
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
	msg_info("\t--help\t\t\t\t:Prints this Message\n");
	msg_info("\t--version\t\t\t:Displays version.\n");
	msg_info("\t--debug\t\t\t\t:Displays enhanced Debug Messages.\n");
	msg_info("\t--dump\t\t\t\t:Implies --debug. Dump the raw data recieved from the device.\n");
	msg_info("\t--device <Device Number>\t:Select device.\n");

	msg_info("\n\tLED:\n");
	msg_info("\t--led <HTML Color Code>\t\t\t:Define Color for LED.\n");
	msg_info("\t--led-warn <HTML Color Code>\t\t:Define Color for Warning Temp.\n");
	msg_info("\t--led-temp <Temperature in Celsius>\t:Define Warning Temperature.\n");

	msg_info("\n\tFan:\n");
	msg_info("\t--fan-temps <CSV of Temperatures>\t:Define Comma Separated Values of Temperatures for Fan.\n");
	msg_info("\t--fan-speeds <CSV of Speed Percentage>\t:Define Comma Separated Values of RPM for Fan.\n");

	msg_info("\n\tPump mode:\n");
	msg_info("\t--pump-mode <mode>\t:set to 3 for quiet, and 5 for performance\n");

	msg_info("\n Without options, OpenCorsairLink will show the status of any detected Corsair Link device.\n");
}
