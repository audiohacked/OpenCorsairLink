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
#include <libusb.h>
#include "common.h"
#include "device.h"
#include "print.h"
#include "scan.h"

extern struct corsair_device_info corsairlink_devices[7];

int scanlist_count = 0;

void corsairlink_close(libusb_context *context)
{
	int r;
	int i;
	for (i=0; i<scanlist_count; i++) {
		r = libusb_release_interface(scanlist[i].handle, 0);
		libusb_close(scanlist[i].handle);
	}
	libusb_exit(context);
}

int corsairlink_device_scanner(libusb_context *context)
{
	int r;
	/* Start: scan code */
	int i; // for loops
	int j; // for loops
	ssize_t cnt;
	struct corsair_device_info *device;
	libusb_device **devices;
	cnt = libusb_get_device_list(context, &devices);
	for (i=0; i<cnt; i++) {
		if (scanlist_count>=10) {
			msg_debug("Limited to 10 CorsairLink devices\n");
			break;
		}
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
				scanlist[scanlist_count].device = &corsairlink_devices[j];
				msg_info("D:%d, CorsairLink Device Found: %s!\n",
					scanlist_count, device->name);
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
	return 0;
}
