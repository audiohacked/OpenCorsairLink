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
#include "../../lowlevel/asetek.h"
#include "../../device.h"
#include "../../driver.h"
#include "../../print.h"
#include "core.h"

int corsairlink_asetekpro_fan_speed(struct corsair_device_info *dev,
            struct libusb_device_handle *handle,
            uint8_t selector, uint16_t *speed, uint16_t *maxspeed)
{
    int rr;
    uint8_t response[32];
    uint8_t commands[32] ;
    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    commands[0] = 0x41; // fan speed query
    commands[1] = selector; // fan port

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 2);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 6);

    msg_debug("%02X %02X %02X %02X %02X %02X\n", response[0], response[1],
                response[2], response[3], response[4], response[5]);

    if (response[0] != 0x41 || response[1] != 0x12
        || response[2] != 0x34 || response[3] != selector)
    {
        msg_debug("Bad Response\n");
    }

    *(speed) = (response[4]<<8) + response[5];
    *(maxspeed) = 0;

    return rr;
}
