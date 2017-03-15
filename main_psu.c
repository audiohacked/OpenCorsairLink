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
	int i; // used in loops

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

	char name[20];
	name[sizeof(name) - 1] = 0;
	uint32_t time = 0;
	uint16_t supply_volts, supply_watts,
		output_volts, output_amps, output_watts; 
	
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

	/* scan for devices */
	corsairlink_device_scanner(context);
	msg_debug("DEBUG: scan done, start routines\n");
	msg_debug("DEBUG: device_number = %d\n", device_number);
	dev = scanlist[device_number].device;
	handle = scanlist[device_number].handle;
	msg_debug("DEBUG: shortcuts set\n"); 

	r = dev->driver->init(handle, dev->write_endpoint);
	msg_debug("DEBUG: init done\n");

	/* fetch device name, vendor name, product name */
	r = dev->driver->name(dev, handle, name);
	msg_info("Name: %s\n", name);
	r = dev->driver->vendor(dev, handle, name);
	msg_info("Vendor: %s\n", name);
	r = dev->driver->product(dev, handle, name);
	msg_info("Product: %s\n", name);
	msg_debug("DEBUG: string done\n");

	/* fetch device powered time and device uptime */
	r = dev->driver->psu_time.powered(dev, handle, &time);
	msg_info("Powered: %u (%dd.  %dh)\n",
		time, time/(24*60*60), time/(60*60)%24);
	r = dev->driver->psu_time.uptime(dev, handle, &time);
	msg_info("Uptime: %u (%dd.  %dh)\n",
		time, time/(24*60*60), time/(60*60)%24);
	msg_debug("DEBUG: time done\n");

	/* fetch Supply Voltage and Total Watts Consumming */
	r = scanlist[device_number].device->driver->power.supply_voltage(scanlist[device_number].device, &supply_volts);
	msg_info("Supply Voltage %5.1f\n", convert_bytes_double(supply_volts));
	r = scanlist[device_number].device->driver->power.total_wattage(scanlist[device_number].device, &supply_watts);
	msg_info("Total Watts %5.1f\n", convert_bytes_double(supply_watts));
	msg_debug("DEBUG: supply done\n");

	/* fetch PSU output */
	for (i=0; i<3; i++) {
		if (i==0)
			msg_info("Output 12v:\n");
		if (i==1)
			msg_info("Output 5v:\n");
		if (i==2)
			msg_info("Output 3.3v:\n");
			
		r = scanlist[device_number].device->driver->power.select(scanlist[device_number].device, i);
		r = scanlist[device_number].device->driver->power.voltage(scanlist[device_number].device, &output_volts);
		msg_info("\tVoltage %5.2f\n", convert_bytes_double(output_volts));

		r = scanlist[device_number].device->driver->power.amperage(scanlist[device_number].device, &output_amps);
		msg_info("\tAmps %5.2f\n", convert_bytes_double(output_amps));

		r = scanlist[device_number].device->driver->power.wattage(scanlist[device_number].device, &output_watts);
		msg_info("\tWatts %5.2f\n", convert_bytes_double(output_watts));
	}
	r = scanlist[device_number].device->driver->power.select(scanlist[device_number].device, 0);

	r = scanlist[device_number].device->driver->deinit(scanlist[device_number].handle, scanlist[device_number].device->write_endpoint);

exit:
	corsairlink_close(context);
	return 0;
}
