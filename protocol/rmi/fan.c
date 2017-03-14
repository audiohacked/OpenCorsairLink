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

/*! \file protocol/rmi/fan.c
 *  \brief Fan Routines for RMi Series of Power Supplies
 */
int corsairlink_rmi_fan_rpm(struct corsair_device_info *dev,
			uint16_t rpm)
{
	int r;
	uint8_t response[64];
	uint8_t commands[32];
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x03;
	commands[1] = 0x90;
	commands[2] = 0x00;
	commands[3] = 0x00;

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, 4);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	rpm = 
	return 0;	
}
