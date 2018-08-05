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
#include "lowlevel/commanderpro.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_commanderpro_device_id(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint8_t* device_id )
{
    // memcpy(device_id, 0x00, 1);
    ( *device_id ) = 0xFF;
    return 0;
}

int
corsairlink_commanderpro_name(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_str_len )
{
    snprintf( name, name_str_len, "%s", dev->name );
    return 0;
}

int
corsairlink_commanderpro_vendor(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_str_len )
{
    snprintf( name, name_str_len, "Corsair" );
    return 0;
}

int
corsairlink_commanderpro_product(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_str_len )
{
    snprintf( name, name_str_len, "%s", dev->name );
    return 0;
}

int
corsairlink_commanderpro_firmware_id(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* firmware,
    uint8_t firmware_str_len )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x02;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    snprintf( firmware, firmware_str_len, "V%d.%d.%d", response[1], response[2], response[3] );

    return rr;
}

int
corsairlink_commanderpro_software_id(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* firmware,
    uint8_t firmware_str_len )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x03;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    snprintf(
        firmware, firmware_str_len, "%d.%d.%d.%d", response[1], response[2], response[3],
        response[4] );

    return rr;
}

int
corsairlink_commanderpro_bootloader_id(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* firmware,
    uint8_t firmware_str_len )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x06;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    snprintf( firmware, firmware_str_len, "V0.%d.%d", response[1], response[2] );

    return rr;
}
