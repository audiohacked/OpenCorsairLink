/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017,2018  Sean Nelson <audiohacked@gmail.com>

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

int corsairlink_commanderpro_get_led_setup_mask(struct corsair_device_info *dev,
			struct libusb_device_handle *handle, uint8_t port)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x30;
	commands[1] = port;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 19);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return rr;
}

int corsairlink_commanderpro_set_led_rgb_colour(struct corsair_device_info *dev,
			struct libusb_device_handle *handle, uint8_t port, struct color *color_led)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x31;
	commands[1] = port;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 19);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return rr;
}

int corsairlink_commanderpro_set_led_rgb_values(struct corsair_device_info *dev,
			struct libusb_device_handle *handle, uint8_t port, struct color *color_led)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x32;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 19);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return rr;
}

int corsairlink_commanderpro_set_commit(struct corsair_device_info *dev,
			struct libusb_device_handle *handle, uint8_t port, struct color *color_led)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x33;
	commands[1] = 0xFF;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 19);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return rr;
}

int corsairlink_commanderpro_set_begin(struct corsair_device_info *dev,
			struct libusb_device_handle *handle, uint8_t port, struct color *color_led)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x34;
	commands[1] = port;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 19);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return rr;
}

int corsairlink_commanderpro_set_led_config(struct corsair_device_info *dev, 
			struct libusb_device_handle *handle, uint8_t channel, uint8_t strip_count,
			uint8_t strip_led_count, uint8_t mode)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x35;
	commands[1] = channel; // CLNP port
	commands[2] = strip_count*strip_led_count;
	commands[3] = strip_led_count;
	commands[4] = mode;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 19);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return rr;
}

int corsairlink_commanderpro_set_led_temperature(struct corsair_device_info *dev,
			struct libusb_device_handle *handle, uint8_t port, struct color *color_led)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x36;
	commands[1] = port;
	commands[2] = port;
	commands[3] = 0x0A;
	commands[4] = 0x28;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 19);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return rr;
}

int corsairlink_commanderpro_set_led_group_reset(struct corsair_device_info *dev,
			struct libusb_device_handle *handle, uint8_t port, struct color *color_led)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x37;
	commands[1] = port;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 19);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return rr;
}

int corsairlink_commanderpro_set_port_state(struct corsair_device_info *dev,
			struct libusb_device_handle *handle, uint8_t port, struct color *color_led)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x38;
	commands[1] = port;
	commands[2] = 0x01;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 19);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return rr;
}

int corsairlink_commanderpro_set_port_brightness(struct corsair_device_info *dev,
			struct libusb_device_handle *handle, uint8_t port, struct color *color_led)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x39;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 19);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return rr;
}

int corsairlink_commanderpro_set_port_led_count(struct corsair_device_info *dev,
			struct libusb_device_handle *handle, uint8_t port, struct color *color_led)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x3A;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 19);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return rr;
}

int corsairlink_commanderpro_set_port_protocol(struct corsair_device_info *dev,
			struct libusb_device_handle *handle, uint8_t port, struct color *color_led)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x3B;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 19);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return rr;
}
