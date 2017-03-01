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

int corsairlink_asetek_device_id(struct corsair_device_info *dev)
{
	return 0x00;
}

int corsairlink_asetek_firmware_id(struct corsair_device_info *dev, char *firmware)
{
	int r;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x10;
	commands[1] = 0x00; //RR
	commands[2] = 0xff; //GG
	commands[3] = 0xff; //BB
	commands[4] = 0x00;
	commands[5] = 0xff;
	commands[6] = 0xff;
	commands[7] = 0xff; //RR
	commands[8] = 0x00; //GG
	commands[9] = 0x00; //BB
	commands[10] = 0x41; // 0x37 = ??, 0x2d = ??
	commands[11] = 0x0a;
	commands[12] = 0x05;
	commands[13] = 0x01;
	commands[14] = 0x00;
	commands[15] = 0x00;
	commands[16] = 0x01;
	commands[17] = 0x00;
	commands[19] = 0x01;

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, 19);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 32);

	sfprintf(firmware, "%d.%d.%d.%d", response[0x17], response[0x18], response[0x19], response[0x1A])

	return r;
}
