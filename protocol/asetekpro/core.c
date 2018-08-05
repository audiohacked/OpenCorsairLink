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

/*! \file protocol/asetek/core.c
 *  \brief Core Routines for RMi Series of Power Supplies
 */
#include "device.h"
#include "driver.h"
#include "lowlevel/asetek.h"
#include "print.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_asetekpro_firmware_id(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* firmware,
    uint8_t firmware_size )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0xAA; // query firmware id

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 1 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 7 );

    msg_debug2(
        "%02X %02X %02X %02X %02X %02X %02X\n", response[0], response[1], response[2], response[3],
        response[4], response[5], response[6] );

    // if (response[0] != 0xAA || response[1] != 0x12
    //     || response[2] != 0x34 || response[3] != selector)
    // {
    //     msg_debug2("Bad Response\n");
    // }

    snprintf(
        firmware, firmware_size, "%d.%d.%d.%d", response[3], response[4], response[5],
        response[6] );

    return rr;
}
