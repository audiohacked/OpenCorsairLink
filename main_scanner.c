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
#include "print.h"
#include "device.h"

extern struct corsair_device_info corsairlink_devices[7];

struct corsair_device_scan {
	struct corsair_device_info *dev;
	struct libusb_device_handle *handle;
} scanlist[10];

int scanlist_count = 0;

int main(int argc, char *argv[])
{
	int r; // result from libusb functions
	int i; // for loops
	int j; // for loops

	struct option_flags flags;
	struct color led_color;
	struct color warning_led;
	struct fan_table fan1;
	struct fan_table pump;
	int8_t warning_led_temp;
	uint8_t pump_mode;

	options_parse(argc, argv, &flags,
		&led_color, &warning_led, &warning_led_temp,
		&fan1, &pump, &pump_mode);

	struct corsair_device_info *device;
	libusb_device **devices;
	libusb_context *context = NULL;

	r = libusb_init(&context);
	if (r < 0) {
		msg_info("Init Error %d\n", r);
		return 1;
	}
	libusb_set_debug(context, 3);

	/* Start: scan code */
	ssize_t cnt;
	cnt = libusb_get_device_list(context, &devices);
	for (i=0; i<cnt; i++) {
		msg_debug("usb device %d\n", i);
		for(j=0; j<7; j++) {
			struct libusb_device_descriptor desc;
			
			msg_debug("corsair device %d\n", j);
			device = &corsairlink_devices[j];

			r = libusb_get_device_descriptor(devices[i], &desc);
			if ((device->vendor_id == desc.idVendor)&&
				(device->product_id == desc.idProduct))
			{
				r = libusb_open(devices[i], &scanlist[scanlist_count].handle);
			}
			if (scanlist[scanlist_count].handle != NULL) {
				msg_info("CorsairLink Device Found: %s!\n",
					device->name);
				r = libusb_detach_kernel_driver(
					scanlist[scanlist_count].handle, 0);
				r = libusb_claim_interface(
					scanlist[scanlist_count].handle, 1);
				scanlist_count++;
				break;
			}
		}
	}
	/* End: scan code */

exit:
	for (i=0; i<scanlist_count; i++) {
		r = libusb_release_interface(scanlist[i].handle, 0);
		libusb_close(scanlist[i].handle);
	}
	libusb_exit(context);
	return 0;
}
