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
#include "../../lowlevel/hid.h"
#include "../../device.h"
#include "../../driver.h"
#include "../../print.h"
#include "core.h"

int corsairlink_hid_fan_mode(struct corsair_device_info *dev, struct libusb_device_handle *handle,
	uint8_t fan_mode)
{
	// int rr;
	// struct fan_table curve;
	// if (fan_mode == PERFORMANCE) {
	// 	HID_FAN_TABLE_EXTREME(curve);
	// }
	// else if (fan_mode == QUIET) {
	// 	HID_FAN_TABLE_QUIET(curve);
	// }
	// r = dev->driver->fan.custom(dev, handle, &curve);
	//
	// return r;

	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	uint8_t ii = 0;

	commands[++ii] = CommandId++;
	commands[++ii] = WriteOneByte;
	commands[++ii] = FAN_Mode;

	if (fan_mode == PERFORMANCE)
		commands[++ii] = HID_Performance;
	else if (fan_mode == BALANCED)
		commands[++ii] = HID_Balanced;
	else if (fan_mode == QUIET)
		commands[++ii] = HID_Quiet;
	else if (fan_mode == DEFAULT)
		commands[++ii] = HID_Default;

	commands[0] = ii;
	rr = dev->driver->write(handle, dev->write_endpoint, commands, ii);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return rr;
}

int corsairlink_hid_fan_curve(struct corsair_device_info *dev, struct libusb_device_handle *handle,
	struct fan_table *fan)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	// commands[0] = FanCurve;
	// commands[1] = UnknownFanCurve;

	uint8_t ii = 0;

	commands[++ii] = CommandId++;
	commands[++ii] = WriteThreeBytes;
	commands[++ii] = FAN_TempTable;
	commands[++ii] = 0x0A;

	commands[++ii] = fan->t1;
	commands[++ii] = 0x00;
	commands[++ii] = fan->t2;
	commands[++ii] = 0x00;
	commands[++ii] = fan->t3;
	commands[++ii] = 0x00;
	commands[++ii] = fan->t4;
	commands[++ii] = 0x00;
	commands[++ii] = fan->t5;
	commands[++ii] = 0x00;

	commands[++ii] = CommandId++;
	commands[++ii] = WriteThreeBytes;
	commands[++ii] = FAN_RPMTable;
	commands[++ii] = 0x0A;

	commands[++ii] = fan->s1;
	commands[++ii] = 0x00;
	commands[++ii] = fan->s2;
	commands[++ii] = 0x00;
	commands[++ii] = fan->s3;
	commands[++ii] = 0x00;
	commands[++ii] = fan->s4;
	commands[++ii] = 0x00;
	commands[++ii] = fan->s5;
	commands[++ii] = 0x00;
	
	commands[0] = ii;
	rr = dev->driver->write(handle, dev->write_endpoint, commands, ii);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return rr;
}

int corsairlink_hid_fan_speed(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t selector, uint16_t *speed)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	uint8_t ii = 0;

	commands[++ii] = CommandId++;
	commands[++ii] = WriteOneByte;
	commands[++ii] = FAN_Select;
	commands[++ii] = selector;

	// commands[++ii] = CommandId++;
	// commands[++ii] = ReadOneByte;
	// commands[++ii] = FAN_Mode;

	commands[++ii] = CommandId++;
	commands[++ii] = ReadTwoBytes;
	commands[++ii] = FAN_ReadRPM;

	commands[0] = ii;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, ii);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	msg_debug("%02X %02X\n", response[0], response[1]);
	*(speed) = (response[0]<<8) + response[1];

	return rr;
}
