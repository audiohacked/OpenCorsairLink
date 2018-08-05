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

#include "device.h"
#include "driver.h"
#include "lowlevel/asetek.h"
#include "print.h"
#include "protocol/asetekpro.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_asetekpro_pump_speed(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x31; // pump speed query

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 1 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 5 );

    msg_debug2(
        "%02X %02X %02X %02X %02X\n", response[0], response[1], response[2], response[3],
        response[4] );

    if ( response[0] != 0x31 || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    ctrl->speed = ( response[3] << 8 ) + response[4];
    ctrl->max_speed = 0;

    return rr;
}
