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
corsairlink_asetekpro_pump_mode_read(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];

    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    msg_debug("function:corsairlink_asetekpro_pump_mode_read file: protocol/asetekpro/pump.c\n");


    commands[0] = AsetekProPumpModeRead;

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 1 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 4 );
  
    msg_debug("pump mode response = %02X %02X %02X %02X\n", response[0], response[1], 
		response[2], response[3]
    );

    if ( response[0] != AsetekProPumpModeRead || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response for astekpro pump mode\n" );
    }

    ctrl->mode = response[3];

    return rr;
}

int
corsairlink_asetekpro_pump_mode_quiet(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = AsetekProPumpModeWrite;
    commands[1] = AsetekProPumpQuiet;

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 5 );

    msg_debug(
        "pump write quiet response = %02X %02X %02X %02X %02X\n", 
	response[0], response[1], response[2], response[3], response[4] );

    rr = corsairlink_asetekpro_pump_mode_read(dev, handle, ctrl);
    return rr;
}

int
corsairlink_asetekpro_pump_mode_balanced(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = AsetekProPumpModeWrite;
    commands[1] = AsetekProPumpBalanced;

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 5 );

    msg_debug(
        "pump write balanced response = %02X %02X %02X %02X %02X\n", 
	response[0], response[1], response[2], response[3], response[4] );

    rr = corsairlink_asetekpro_pump_mode_read(dev, handle, ctrl);
    return rr;
}

int
corsairlink_asetekpro_pump_mode_performance(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = AsetekProPumpModeWrite;
    commands[1] = AsetekProPumpPerformance;

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 5 );

    msg_debug(
        "pump write performance response = %02X %02X %02X %02X %02X\n", 
	response[0], response[1], response[2], response[3], response[4] );

    rr = corsairlink_asetekpro_pump_mode_read(dev, handle, ctrl);
    return rr;
}

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

    commands[0] = AsetekProPumpSpeedRead;

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 1 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 5 );

    msg_debug(
        "pump speed response = %02X %02X %02X %02X %02X\n", response[0], response[1], response[2], response[3],
        response[4] );

    if ( response[0] != AsetekProPumpSpeedRead || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response for astekpro pump speed\n" );
    }

    ctrl->speed = ( response[3] << 8 ) + response[4];
    ctrl->max_speed = 0;

    return rr;
}
