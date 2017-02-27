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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libusb.h>
#include "options.h"
#include "device.h"
#include "print.h"
#include "scan.h"
#include "lowlevel/asetek4.h"
#include "lowlevel/hid.h"
#include "lowlevel/rmi.h"
#include "protocol/hid/core.h"
#include "protocol/rmi/core.h"

int main(int argc, char *argv[])
{
	int r; // result from libusb functions

	uint8_t device_number = 0;
	struct option_flags flags;
	struct color led_color;
	struct color warning_led;
	struct fan_table fan1;
	struct fan_table pump;
	int8_t warning_led_temp;
	uint8_t pump_mode;

	options_parse(argc, argv, &flags, &device_number,
		&led_color, &warning_led, &warning_led_temp,
		&fan1, &pump, &pump_mode);
	
	libusb_context *context = NULL;

	r = libusb_init(&context);
	if (r < 0) {
		msg_info("Init Error %d\n", r);
		return 1;
	}
	libusb_set_debug(context, 3);

	corsairlink_device_scanner(context);

	char name[20];
	name[sizeof(name) - 1] = 0;
	uint32_t time = 0;
	uint16_t supply_volts, supply_watts,
		output_volts, output_amps, output_watts; 
	
	r = scanlist[device_number].device->driver->init(scanlist[device_number].handle, scanlist[device_number].device->write_endpoint);

	/* fetch device name, vendor name, product name */
	r = scanlist[device_number].device->driver->name(scanlist[device_number].device, name);
	msg_info("Name: %s\n", name);
	r = scanlist[device_number].device->driver->vendor(scanlist[device_number].device, name);
	msg_info("Vendor: %s\n", name);
	r = scanlist[device_number].device->driver->product(scanlist[device_number].device, name);
	msg_info("Product: %s\n", name);

	r = scanlist[device_number].device->driver->led(scanlist[device_number].device, &led_color, &warning_led, warning_led_temp, (warning_led_temp > -1));

	r = scanlist[device_number].device->driver->deinit(scanlist[device_number].handle, scanlist[device_number].device->write_endpoint);

exit:
	corsairlink_close(context);
	return 0;
}
