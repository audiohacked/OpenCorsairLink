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
#include "../../lowlevel/asetek4.h"
#include "../../device.h"
#include "../../driver.h"


int corsairlink_asetek_change_led(struct corsair_device_info *dev,
			struct libusb_device_handle *handle, struct color *color_led,
			struct color *warning_led, uint8_t Warning_Temp, uint8_t Warning_Enable)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x10;
	commands[1] = color_led->red;
	commands[2] = color_led->green;
	commands[3] = color_led->blue;
	commands[4] = 0x00;
	commands[5] = 0xff;
	commands[6] = 0xff;
	commands[7] = warning_led->red;
	commands[8] = warning_led->green;
	commands[9] = warning_led->blue;
	commands[10] = Warning_Temp; // 0x37 = ??, 0x2d = ??
	commands[11] = 0x0a;
	commands[12] = 0x05;
	commands[13] = 0x01;
	commands[14] = 0x00;
	commands[15] = 0x00;
	commands[16] = Warning_Enable;
	commands[17] = 0x00;
	commands[18] = 0x01;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 19);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	// fan_rpm = (long int) response[0]*16*16 + response[1];
	// pump_rpm = (response[8]*16*16)+response[9];
	// liquid_temp = (double) response[10] + (double) response[14]/10;

	return rr;
}
