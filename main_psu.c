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
	struct corsair_device_info *dev;
	int r; // result from libusb functions
	int i; // for loops

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
	
	options_parse(argc, argv, &flags,
		&led_color, &warning_led, &warning_led_temp,
		&fan1, &pump, &pump_mode);

	/* scan for devices */

	r = dev->driver->init(dev->handle, dev->write_endpoint);

	/* fetch device name, vendor name, product name */
	r = dev->driver->name(dev, name);
	msg_info("Name: %s\n", name);
	r = dev->driver->vendor(dev, name);
	msg_info("Vendor: %s\n", name);
	r = dev->driver->product(dev, name);
	msg_info("Product: %s\n", name);

	/* fetch device powered time and device uptime */
	r = dev->driver->psu_time.powered(dev, &time);
	msg_info("Powered: %u (%dd.  %dh)\n",
		time, time/(24*60*60), time/(60*60)%24);
	r = dev->driver->psu_time.uptime(dev, &time);
	msg_info("Uptime: %u (%dd.  %dh)\n",
		time, time/(24*60*60), time/(60*60)%24);

	/* fetch Supply Voltage and Total Watts Consumming */
	r = dev->driver->power.supply_voltage(dev, &supply_volts);
	msg_info("Supply Voltage %5.1f\n", convert_bytes_double(supply_volts));
	r = dev->driver->power.total_wattage(dev, &supply_watts);
	msg_info("Total Watts %5.1f\n", convert_bytes_double(supply_watts));

	/* fetch PSU output */
	for (i=0; i<3; i++) {
		if (i==0)
			msg_info("Output 12v:\n");
		if (i==1)
			msg_info("Output 5v:\n");
		if (i==2)
			msg_info("Output 3.3v:\n");
			
		r = dev->driver->power.select(dev, i);
		r = dev->driver->power.voltage(dev, &output_volts);
		msg_info("\tVoltage %5.2f\n", convert_bytes_double(output_volts));

		r = dev->driver->power.amperage(dev, &output_amps);
		msg_info("\tAmps %5.2f\n", convert_bytes_double(output_amps));

		r = dev->driver->power.wattage(dev, &output_watts);
		msg_info("\tWatts %5.2f\n", convert_bytes_double(output_watts));
	}
	r = dev->driver->power.select(dev, 0);

	r = dev->driver->deinit(dev->handle, dev->write_endpoint);

exit:
	corsairlink_close(dev);
	return 0;
}
