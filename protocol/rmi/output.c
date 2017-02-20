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

	commands[0] = 0x02; // Length
	commands[1] = 0x00; // Command Opcode: Output Select
	commands[2] = output_select; // Command data...

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, 2);
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

	commands[0] = 0x03; // Length
	commands[1] = 0x8B; // Command Opcode: Output Select
	commands[2] = 0x00; // Command data...
	commands[3] = 0x00;

	*volts = 0;

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, 4);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	*volts = ((response[2]<<8)+response[3]);

	return 0;
}

int corsairlink_rmi_output_amps(struct corsair_device_info *dev, float *amps) {
	int r;
	uint8_t response[64];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	uint8_t i = 1;

	commands[0] = 0x03; // Length
	commands[1] = 0x8C; // Command Opcode: Output Select
	commands[2] = 0x00; // Command data...
	commands[3] = 0x00;

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, 4);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	*amps = ((response[2]<<8) + response[3]);

	return 0;
}

int corsairlink_rmi_output_watts(struct corsair_device_info *dev, float *watts) {
	int r;
	uint8_t response[64];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	uint8_t i = 1;

	commands[0] = 0x03; // Length
	commands[1] = 0x96; // Command Opcode: Output Select
	commands[2] = 0x00; // Command data...
	commands[3] = 0x00;

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, 4);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	*watts = ((response[2]<<8)+ response[3]);

	return 0;
}

