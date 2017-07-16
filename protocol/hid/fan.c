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
#include "core.h"

int corsairlink_hid_fan_mode(struct corsair_device_info *dev, struct libusb_device_handle *handle,
	uint8_t fan_mode)
{
	int r;
	struct fan_table curve;
	if (fan_mode == PERFORMANCE) {
		HID_FAN_TABLE_EXTREME(curve);
	}
	else if (fan_mode == QUIET) {
		HID_FAN_TABLE_QUIET(curve);
	}
	r = dev->driver->fan.custom(dev, handle, &curve);

	return r;
}

int corsairlink_hid_fan_curve(struct corsair_device_info *dev, struct libusb_device_handle *handle,
	struct fan_table *fan)
{
	int r;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = FanCurve;
	commands[1] = UnknownFanCurve;
	
	uint8_t i = 1;

	i = 1;
	commands[i++] = CommandId++
	commands[i++] = WriteThreeBytes;
	commands[i++] = FAN_TempTable;
	commands[i++] = 0x0A;

	commands[i++] = fan->t1;
	commands[i++] = 0x00;
	commands[i++] = fan->t2;
	commands[i++] = 0x00;
	commands[i++] = fan->t3;
	commands[i++] = 0x00;
	commands[i++] = fan->t4;
	commands[i++] = 0x00;
	commands[i++] = fan->t5;
	commands[i++] = 0x00;

	commands[i++] = CommandId++
	commands[i++] = WriteThreeBytes;
	commands[i++] = FAN_RPMTable;
	commands[i++] = 0x0A;

	commands[i++] = fan->s1;
	commands[i++] = 0x00;
	commands[i++] = fan->s2;
	commands[i++] = 0x00;
	commands[i++] = fan->s3;
	commands[i++] = 0x00;
	commands[i++] = fan->s4;
	commands[i++] = 0x00;
	commands[i++] = fan->s5;
	commands[i++] = 0x00;
	
	commands[0] = i;
	r = dev->driver->write(handle, dev->write_endpoint, commands, i);
	r = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return r;
}

int corsairlink_hid_fan_speed(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t selector, uint16_t *speed)
{
	int r;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	uint8_t i = 1;

	i = 1;
	commands[i++] = CommandId++;
	commands[i++] = WriteOneByte;
	commands[i++] = FAN_Select;
	commands[i++] = 0x00;

	// commands[i++] = CommandId++;
	// commands[i++] = ReadOneByte;
	// commands[i++] = FAN_Mode;

	commands[i++] = CommandId++;
	commands[i++] = ReadTwoBytes;
	commands[i++] = FAN_ReadRPM;

	commands[0] = i;

	r = dev->driver->write(handle, dev->write_endpoint, commands, i);
	r = dev->driver->read(handle, dev->read_endpoint, response, 32);

	msg_debug("%02X %02X\n", response[0], response[1]);
	*(speed) = (response[0]<<8) + response[1];

	return r;
}
