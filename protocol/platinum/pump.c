/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017-2019  Sean Nelson <audiohacked@gmail.com>

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
#include "logic/print.h"
#include "logic/options.h"
#include "lowlevel/platinum.h"
#include "protocol/platinum.h"
#include "protocol/crc.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_platinum_pump_mode_read(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    // uint8_t response[64];
    // uint8_t commands[64];
    //
    // memset( response, 0, sizeof( response ) );
    // memset( commands, 0, sizeof( commands ) );
    //
    // uint8_t ii = 0;
    //
    // commands[++ii] = CommandId++;
    // commands[++ii] = WriteOneByte;
    // commands[++ii] = FAN_Select;
    // commands[++ii] = ctrl->channel;
    //
    // commands[++ii] = CommandId++;
    // commands[++ii] = ReadOneByte;
    // commands[++ii] = FAN_Mode;
    //
    // commands[0] = ii;
    //
    // rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    // rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 64 );

    ctrl->mode = 0x00;

    return rr;
}

int
corsairlink_platinum_pump_speed(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0x00] = 0x3F;
    commands[0x01] = 0x78;

    commands[0x3F] = crc8ccitt(commands+1, 62);

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 64 );

    msg_debug2( "Speed: %02X %02X\n", response[0x1D], response[0x1E] );
    msg_debug2( "Max Speed: %02X %02X\n", response[9], response[8] );

    ctrl->speed = ( response[0x1E] << 8 ) + response[0x1D];
    ctrl->max_speed = 0xFFFF;

    return rr;
}
