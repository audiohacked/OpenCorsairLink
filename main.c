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

	uint8_t device_number;
	struct corsair_device_info *dev;
	struct libusb_device_handle *handle;

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
	msg_debug("DEBUG: scan done, start routines\n");
	msg_debug("DEBUG: device_number = %d\n", device_number);

	dev = scanlist[device_number].device;
	handle = scanlist[device_number].handle;
	msg_debug("DEBUG: shortcuts set\n");

	r = dev->driver->init(handle, dev->write_endpoint);
	msg_debug("DEBUG: init done\n");

	/* fetch device name, vendor name, product name */
	//r = dev->driver->name(dev, name);
	//msg_info("Name: %s\n", name);
	//r = dev->driver->vendor(dev, name);
	//msg_info("Vendor: %s\n", name);
	//r = dev->driver->product(dev, name);
	//msg_info("Product: %s\n", name);

	r = dev->driver->led(dev, &led_color, &warning_led, warning_led_temp, (warning_led_temp > -1));

	r = dev->driver->deinit(handle, dev->write_endpoint);
	msg_debug("DEBUG: deinit done\n");

exit:
	corsairlink_close(context);
	return 0;
}
