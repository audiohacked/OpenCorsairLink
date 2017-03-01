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
#include <math.h>

#include "../../lowlevel/hid.h"
#include "../../device.h"
#include "../../driver.h"
#include "core.h"

double convert_bytes_double(uint16_t v16)
{
	uint8_t d0 = (v16&0xFF00)>>8;
	uint8_t d1 = v16&0xFF;

	int p = (d1 >> 3) & 0x1F;
	if (p > 0xF) p -= 32;

	int v = (((int)d1& 0x7)<<8) + (int)d0;
	if (v > 0x400) v = -(0x10000 - (v|0xF800));

	return (double)v * pow(2.0, (double)p);
}

int corsairlink_rmi_device_id(struct corsair_device_info *dev)
{
	int r;
	uint8_t response[64];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	uint8_t i = 1;

	i = 1;
	// Read Device ID: 0x3b = H80i. 0x3c = H100i. 0x41 = H110i. 0x42 = H110i Extreme
	commands[i++] = 0x03; // Command Opcode
	commands[i++] = 0x00; // Command data...
	commands[i++] = 0x00;

	commands[0] = 0xfe; // Length

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, i);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	return response[2];
}

int corsairlink_rmi_firmware_id(struct corsair_device_info *dev)
{
	int r;
	uint8_t response[64];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	uint8_t i = 1;

	i = 1;
	commands[i++] = 0x00; // Command Opcode
	commands[i++] = 0x00; // Command data...
	commands[i++] = 0x00;

	commands[0] = i; // Length

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, i);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	int firmware = response[3]<<8;
	firmware += response[2];
	return firmware;
}

int corsairlink_rmi_name(struct corsair_device_info *dev,
			char *name)
{
	int r;
	uint8_t response[64];
	uint8_t commands[32];
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0xfe;
	commands[1] = 0x03;
	commands[2] = 0x00;

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, 2);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	memcpy(name, response+2, 16);

	return 0;
}

int corsairlink_rmi_vendor(struct corsair_device_info *dev,
			char *name)
{
	int r;
	uint8_t response[64];
	uint8_t commands[32];
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x03;
	commands[1] = 0x99;
	commands[2] = 0x00;

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, 2);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	memcpy(name, response+2, 16);

	return 0;
}

int corsairlink_rmi_product(struct corsair_device_info *dev,
			char *name)
{
	int r;
	uint8_t response[64];
	uint8_t commands[32];
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x03;
	commands[1] = 0x9a;
	commands[2] = 0x00;

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, 2);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	memcpy(name, response+2, 16);

	return 0;
}

