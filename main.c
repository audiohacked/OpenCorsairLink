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
#include "driver.h"
#include "print.h"
#include "scan.h"
#include "lowlevel/asetek4.h"
#include "lowlevel/hid.h"
#include "lowlevel/rmi.h"
#include "protocol/hid/core.h"
#include "protocol/rmi/core.h"

int psu_settings(struct corsair_device_scan scanned_device, struct option_parse_return settings) {
	int rr;
	int ii;
	char name[20];
	name[sizeof(name) - 1] = 0;
	uint32_t time = 0;
	uint16_t supply_volts, supply_watts, temperature,
		output_volts, output_amps, output_watts; 
	struct corsair_device_info *dev;
	struct libusb_device_handle *handle;

	dev = scanned_device.device;
	handle = scanned_device.handle;
	msg_debug("DEBUG: shortcuts set\n");

	rr = dev->driver->init(handle, dev->write_endpoint);
	msg_debug("DEBUG: init done\n");

	/* fetch device name, vendor name, product name */
	rr = dev->driver->name(dev, handle, name);
	rr = dev->driver->vendor(dev, handle, name);
	msg_info("Vendor: %s\n", name);
	rr = dev->driver->product(dev, handle, name);
	msg_info("Product: %s\n", name);
	rr = dev->driver->fw_version(dev, handle, name);
	msg_info("Firmware: %s\n", name);
	msg_debug("DEBUG: string done\n");

	/* fetch temperatures */
	for (ii=0; ii<2; ii++) {
		rr = dev->driver->temperature(dev, handle, ii, &temperature);
		msg_info("Temperature %d: %5.2f C\n", ii, convert_bytes_double(temperature));
	}

	/* fetch device powered time and device uptime */
	rr = dev->driver->psu_time.powered(dev, handle, &time);
	msg_info("Powered: %u (%dd.  %dh)\n",
		time, time/(24*60*60), time/(60*60)%24);
	rr = dev->driver->psu_time.uptime(dev, handle, &time);
	msg_info("Uptime: %u (%dd.  %dh)\n",
		time, time/(24*60*60), time/(60*60)%24);
	msg_debug("DEBUG: time done\n");

	/* fetch Supply Voltage and Total Watts Consumming */
	rr = dev->driver->power.supply_voltage(dev, handle, &supply_volts);
	msg_info("Supply Voltage: %5.2f\n", convert_bytes_double(supply_volts));
	rr = dev->driver->power.total_wattage(dev, handle, &supply_watts);
	msg_info("Total Watts: %5.2f\n", convert_bytes_double(supply_watts));
	msg_debug("DEBUG: supply done\n");

	/* fetch PSU output */
	for (ii=0; ii<3; ii++) {
		if (ii==0)
			msg_info("Output 12v:\n");
		if (ii==1)
			msg_info("Output 5v:\n");
		if (ii==2)
			msg_info("Output 3.3v:\n");
			
		rr = dev->driver->power.select(dev, handle, ii);
		rr = dev->driver->power.voltage(dev, handle, &output_volts);
		msg_info("\tVoltage %5.2f\n", convert_bytes_double(output_volts));

		rr = dev->driver->power.amperage(dev, handle, &output_amps);
		msg_info("\tAmps %5.2f\n", convert_bytes_double(output_amps));

		rr = dev->driver->power.wattage(dev, handle, &output_watts);
		msg_info("\tWatts %5.2f\n", convert_bytes_double(output_watts));
	}
	rr = dev->driver->power.select(dev, handle, 0);

	rr = dev->driver->deinit(handle, dev->write_endpoint);

	return 0;
}

int hydro_settings(struct corsair_device_scan scanned_device, struct option_parse_return settings) {
	int rr;
	int ii;
	char name[20];
	name[sizeof(name) - 1] = 0;
	struct corsair_device_info *dev;
	struct libusb_device_handle *handle;
	uint16_t temperature;
	double celsius = 0;
	uint16_t pump_speed = 0;
	uint16_t fan_speed = 0;

	dev = scanned_device.device;
	handle = scanned_device.handle;
	msg_debug("DEBUG: shortcuts set\n");

	rr = dev->driver->init(handle, dev->write_endpoint);
	msg_debug("DEBUG: init done\n");

	/* fetch device name, vendor name, product name */
	rr = dev->driver->vendor(dev, handle, name);
	msg_info("Vendor: %s\n", name);
	rr = dev->driver->product(dev, handle, name);
	msg_info("Product: %s\n", name);
	rr = dev->driver->fw_version(dev, handle, name);
	msg_info("Firmware: %s\n", name);

	for (ii=0; ii<3; ii++) {
		rr = dev->driver->temperature(dev, handle, ii, &temperature);
		if (dev->driver == &corsairlink_driver_asetek) {
			uint8_t v1 = (temperature>>8);
			uint8_t v2 = (temperature&0xFF);
			msg_debug("DEBUG: %02X %02X\n", v1, v2);
			celsius = (double)v1 + ((double)v2/10);
		} else if (dev->driver == &corsairlink_driver_hid) {
			celsius = (double)temperature / 256;
		}
		msg_info("Temperature %d: %5.2f\n", ii, (double)celsius);
		if (dev->driver == &corsairlink_driver_asetek) {
			break;
		} 
	}

	for (ii=0; ii<dev->fan_control_count; ii++) {
		rr = dev->driver->fan.speed(dev, handle, ii, &fan_speed);
		msg_info("Fan Speed %d: %i\n", ii, fan_speed);
	}
	
	rr = dev->driver->pump.speed(dev, handle, dev->pump_index, &pump_speed);
	msg_info("Pump Speed: %i\n", pump_speed);

	rr = dev->driver->led(dev, handle, &settings.led_color, &settings.warning_led, settings.warning_led_temp, (settings.warning_led_temp > -1));

	if (dev->driver == &corsairlink_driver_asetek) {
		if (settings.fan1.s6 != 0)
			dev->driver->fan.custom(dev, handle, &settings.fan1);
		if (settings.pump_mode != DEFAULT)
			dev->driver->pump.profile(dev, handle, settings.pump_mode);
	}

	rr = dev->driver->deinit(handle, dev->write_endpoint);
	msg_debug("DEBUG: deinit done\n");
	
	return 0;
}

int main(int argc, char *argv[])
{
	int rr; // result from libusb functions

	int8_t device_number = -1;

	struct option_flags flags;
	struct option_parse_return settings;

	options_parse(argc, argv, &flags, &device_number,
		&settings);
	
	libusb_context *context = NULL;

	rr = libusb_init(&context);
	if (rr < 0) {
		msg_info("Init Error %d\n", rr);
		return 1;
	}
	libusb_set_debug(context, 3);

	corsairlink_device_scanner(context);
	msg_debug("DEBUG: scan done, start routines\n");
	msg_debug("DEBUG: device_number = %d\n", device_number);

	if (device_number >= 0) {
		if (scanlist[device_number].device->driver == &corsairlink_driver_rmi) {
			psu_settings(scanlist[device_number], settings);
		} else {
			hydro_settings(scanlist[device_number], settings);
		}
	}

exit:
	corsairlink_close(context);
	return 0;
}
