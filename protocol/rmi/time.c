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

/*! \file protocol/rmi/time.c
 *  \brief Uptime Routines for RMi Series of Power Supplies
 */
#include "device.h"
#include "driver.h"
#include "lowlevel/rmi.h"
#include "protocol/rmi.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_rmi_time_powered(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint32_t* powered )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x03;
    commands[1] = 0xd1;
    commands[2] = 0x00;
    commands[3] = 0x00;
    commands[4] = 0x00;
    commands[5] = 0x00;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    memcpy( powered, response + 2, 4 );

    return 0;
}

int
corsairlink_rmi_time_uptime(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint32_t* uptime )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x03;
    commands[1] = 0xd2;
    commands[2] = 0x00;
    commands[3] = 0x00;
    commands[4] = 0x00;
    commands[5] = 0x00;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    memcpy( uptime, response + 2, 4 );

    return 0;
}
