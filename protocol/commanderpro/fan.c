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
#include "../../lowlevel/commanderpro.h"
#include "../../device.h"
#include "../../driver.h"
#include "../../print.h"
#include "core.h"

int corsairlink_commanderpro_get_fan_setup_mask(struct corsair_device_info *dev,
            struct libusb_device_handle *handle,
            uint8_t sensor_index)
{
	int rr;
	uint8_t response[16];
	uint8_t commands[64] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x21;
    commands[1] = sensor_index;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 14);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

    for (int ii = 1; ii <= 6; ++ii)
    {
        switch(response[ii])
        {
            case 0x00:
                msg_debug("Auto/Disconnected");
                break;
            case 0x01:
                msg_debug("3-Pin");
                break;
            case 0x02:
                msg_debug("4-Pin");
                break;
            case 0x03:
                msg_debug("Unknown");
                break;
            default:
                msg_debug("Impossible!");
        }
    }


	return rr;
}

int corsairlink_commanderpro_get_fan_speed_rpm(struct corsair_device_info *dev,
            struct libusb_device_handle *handle,
            uint8_t sensor_index,
            uint16_t *rpm,
            uint16_t *maxrpm)
{
	int rr;
	uint8_t response[16];
	uint8_t commands[64] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x21;
    commands[1] = sensor_index;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 14);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

    *(rpm) = (response[1]<<8) + response[2];

	return rr;
}

int corsairlink_commanderpro_get_fan_speed_pwm(struct corsair_device_info *dev,
			struct libusb_device_handle *handle,
            uint8_t sensor_index,
            uint8_t *pwm)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x22;
    commands[1] = sensor_index;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 14);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

    *(pwm) = response[1];

	return rr;
}

int corsairlink_commanderpro_get_fan_detect_type(struct corsair_device_info *dev,
			struct libusb_device_handle *handle,
            uint8_t sensor_index,
            uint8_t *device_id)
{
	int rr;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x29;
    commands[1] = sensor_index;

	rr = dev->driver->write(handle, dev->write_endpoint, commands, 14);
	rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return rr;
}
