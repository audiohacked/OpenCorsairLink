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

int corsairlink_hid_device_id(struct corsair_device_info *dev, uint8_t *device_id)
{
	int r;
	uint8_t response[64];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	uint8_t i = 1;

	i = 1;
	// Read Device ID: 0x3b = H80i. 0x3c = H100i. 0x41 = H110i. 0x42 = H110i Extreme
	commands[i++] = CommandId++; // Command ID
	commands[i++] = ReadOneByte; // Command Opcode
	commands[i++] = DeviceID; // Command data...
	commands[i++] = 0x00;

	commands[0] = i; // Length

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, i);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	memcpy(device_id, response+2, 1);

	return 0;
}

int corsairlink_hid_name(struct corsair_device_info *dev, char *name)
{
	sprintf(name, "%s", dev->name);
	return 0;
}

int corsairlink_hid_vendor(struct corsair_device_info *dev, char *name)
{
	sprintf(name,"Corsair");
	return 0;
}

int corsairlink_hid_product(struct corsair_device_info *dev, char *name)
{
	sprintf(name, "%s", dev->name);
	return 0;
}

int corsairlink_hid_firmware_id(struct corsair_device_info *dev, char *firmware)
{
	int r;
	uint8_t response[64];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	uint8_t i = 1;

	i = 1;
	commands[i++] = CommandId++; // Command ID
	commands[i++] = ReadTwoBytes; // Command Opcode
	commands[i++] = FirmwareID; // Command data...
	commands[i++] = 0x00;

	commands[0] = i; // Length

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, i);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	sprintf(firmware, "%d.%d.0.0", response[3], response[2]);

	return 0;
}
