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
#include "core.h"

int corsairlink_rmi_output_select(struct corsair_device_info *dev, uint8_t output_select) {
	int r;
	uint8_t response[64];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	uint8_t i = 1;

	commands[i++] = 0x00; // Command Opcode: Output Select
	commands[i++] = output_select; // Command data...

	commands[0] = i; // Length

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, i);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	return 0;
}

int corsairlink_rmi_output_volts(struct corsair_device_info *dev, float *volts) {
	int r;
	uint8_t response[64];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	uint8_t i = 1;

	commands[i++] = 0x8B; // Command Opcode: Output Select
	commands[i++] = 0x00; // Command data...
	commands[i++] = 0x00;

	commands[0] = i; // Length

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, i);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	*volts = (float)(response[4]<<8);
	*volts += response[3];

	return 0;
}

int corsairlink_rmi_output_amps(struct corsair_device_info *dev, float *amps) {
	int r;
	uint8_t response[64];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	uint8_t i = 1;

	commands[i++] = 0x8C; // Command Opcode: Output Select
	commands[i++] = 0x00; // Command data...
	commands[i++] = 0x00;

	commands[0] = i; // Length

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, i);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	*amps = (float)(response[4]<<8);
	*amps += response[3];

	return 0;
}

int corsairlink_rmi_output_watts(struct corsair_device_info *dev, float *watts) {
	int r;
	uint8_t response[64];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	uint8_t i = 1;

	commands[i++] = 0x96; // Command Opcode: Output Select
	commands[i++] = 0x00; // Command data...
	commands[i++] = 0x00;

	commands[0] = i; // Length

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, i);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	*watts = (float)(response[4]<<8);
	*watts += response[3];

	return 0;
}

