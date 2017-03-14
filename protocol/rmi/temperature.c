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

/*! \file protocol/rmi/temperature.c
 *  \brief Temperature Routines for RMi Series of Power Supplies
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libusb.h>

#include "../../lowlevel/rmi.h"
#include "../../device.h"
#include "../../driver.h"
#include "../../print.h"
#include "core.h"

int corsairlink_rmi_temperature(struct corsair_device_info *dev,
			uint8_t select, uint16_t *temperature)
{
	int r;
	uint8_t response[64];
	uint8_t commands[32];
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x03;
	commands[1] = 0x8D + select;
	commands[2] = 0x00;
	commands[3] = 0x00;

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, 4);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	memcpy(temperature, response+2, 2);

	msg_debug("%02X %02X %02X %02X %02X %02X\n",
		response[0], response[1], response[2],
		response[3], response[4], response[5]);

	return 0;
}

