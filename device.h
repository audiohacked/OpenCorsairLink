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

#ifndef _DEVICE_H
#define _DEVICE_H

#include "common.h"

struct corsair_device_info {
	/** device info */
	char name[16];
	uint16_t vendor_id;
	uint16_t product_id;
	uint16_t device_id;

	/** device endpoints */
	uint8_t read_endpoint;
	uint8_t write_endpoint;

	/** libusb device structures */
	struct libusb_device_handle *handle;
	libusb_context *context;
	int interface;

	/** device control info */
	struct corsair_device_driver *driver;
	uint8_t led_control_count; // used mostly with HID driver
	uint8_t fan_control_count; // used with HID driver
	uint8_t pump_index; // used with HID driver
};

struct corsair_device_driver {
	/** device communication helper functions */
	int (*init)(struct libusb_device_handle*, uint8_t);
	int (*deinit)(struct libusb_device_handle*, uint8_t);
	int (*read)(struct libusb_device_handle*, uint8_t, uint8_t*, int);
	int (*write)(struct libusb_device_handle*, uint8_t, uint8_t*, int);
	
	/** protocol functions */
	int (*device_id)(struct corsair_device_info *dev);
	int (*fw_version)(struct corsair_device_info *dev);
	int (*led)(struct corsair_device_info *dev, struct color *l, struct color *w, uint8_t t, uint8_t e);

	struct fan_functions {
		int (*profile)(struct corsair_device_info *dev, uint8_t profile);
		int (*custom)(struct corsair_device_info *dev, struct fan_table *custom_profile);
	} fan_control;

	struct pump_functions {
		int (*profile)(struct corsair_device_info *dev, uint8_t profile);
		int (*custom)(struct corsair_device_info *dev, struct fan_table *custom_profile);
	} pump_control;

	struct power_functions {
		int (*select)(struct corsair_device_info *dev, uint8_t output_select);
		int (*voltage)(struct corsair_device_info *dev, float *volts);
		int (*amperage)(struct corsair_device_info *dev, float *amps);
		int (*wattage)(struct corsair_device_info *dev, float *watts);
	} power;

	int (*fan_speed_read)();
};

#endif

