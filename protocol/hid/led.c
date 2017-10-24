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

int corsairlink_hid_change_led(struct corsair_device_info *dev, struct libusb_device_handle *handle, struct color *color_led, struct color *warning_led,
			uint8_t Warning_Temp, uint8_t Warning_Enable)
{
	int rr;
	uint8_t response[64];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	uint8_t ii = 0;

	commands[++ii] = CommandId++; // Command ID
	commands[++ii] = WriteOneByte; // Command Opcode
	commands[++ii] = LED_SelectCurrent; // Command data...
	commands[++ii] = 0;

	commands[++ii] = CommandId++; // Command ID
	commands[++ii] = WriteOneByte; // Command Opcode
	commands[++ii] = LED_Mode; // Command data...
	commands[++ii] = 0x00;

	commands[++ii] = CommandId++; // Command ID
	commands[++ii] = WriteThreeBytes; // Command Opcode
	commands[++ii] = LED_CycleColors; // Command data...
	commands[++ii] = 0x0C;

	commands[++ii] = color_led->red;
	commands[++ii] = color_led->green;
	commands[++ii] = color_led->blue;

	commands[++ii] = color_led->red;
	commands[++ii] = color_led->green;
	commands[++ii] = color_led->blue;

	commands[++ii] = color_led->red;
	commands[++ii] = color_led->green;
	commands[++ii] = color_led->blue;

	commands[++ii] = color_led->red;
	commands[++ii] = color_led->green;
	commands[++ii] = color_led->blue;

	commands[0] = ii; // Length

	rr = dev->driver->write(handle, dev->write_endpoint, commands, ii);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 64);

	// fan_rpm = (long int) response[0]*16*16 + response[1];
	// pump_rpm = (response[8]*16*16)+response[9];
	// liquid_temp = (double) response[10] + (double) response[14]/10;

	return rr;
}
