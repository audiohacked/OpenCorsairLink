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

/*! \file protocol/axi/core.c
 *  \brief Core Routines for AXi Series of Power Supplies
 */
#include "device.h"
#include "driver.h"
#include "lowlevel/axi.h"
#include "protocol/axi.h"

#include <errno.h>
#include <libusb.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

double
convert_bytes_double( uint16_t v16 )
{
    int exponent = v16 >> 11;
    int fraction = (int)( v16 & 2047 );
    if ( exponent > 15 )
        exponent = -( 32 - exponent );

    if ( fraction > 1023 )
        fraction = -( 2048 - fraction );

    if ( ( fraction & 1 ) == 1 )
        fraction++;

    return (double)fraction * pow( 2.0, (double)exponent );
}

/*
float convert_byte_float(unsigned char * data) {

    int p1 = (data[1] >> 3) & 31;
    if (p1 > 15) p1 -= 32;

    int p2 = ((int)data[1] & 7) * 256 + (int)data[0];
    if (p2 > 1024) p2 = -(65536 - (p2 | 63488));

    return (float) p2 * powf(2.0, (float) p1);
}

void convert_float_byte(float val, int exp, unsigned char *data) {
    int p1;
    if (val > 0.0) {
        p1 = round(val * pow(2.0, (double) exp));
        if (p1 > 1023) p1 = 1023;
    } else {
        int p2 = round(val * pow(2.0, (double) exp));
        if (p2 < -1023) p2 = -1023;
        p1 = p2 & 2047;
    }
    data[0] = (unsigned char) (p1 & 0xff);
    exp = exp <= 0 ? -exp : 256 - exp;
    exp = exp << 3 & 255;
    data[1] = (unsigned char) (((p1 >> 8) & 0xff) | exp);
}
*/

int
corsairlink_axi_device_id(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint8_t* device_id )
{
    // memset(device_id, 0x00, 1);
    ( *device_id ) = 0xFF;
    return 0;
}

int
corsairlink_axi_firmware_id(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* firmware,
    uint8_t firmware_size )
{
    snprintf( firmware, firmware_size, "NA" );

    return 0;
}

int
corsairlink_axi_name(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_size )
{
    int rr;
    int ii = 0;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );
    
    int dev_strlen = 7;

    commands[0] = 0x13; // magic numer 19
    commands[1] = 0x03;
    commands[2] = 0x06;
    commands[3] = 0x01;
    commands[4] = 0x07;
    commands[5] = dev_strlen; // len
    commands[6] = 0x9a; // reg

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    commands[0] = 0x08; // magic numer 19
    commands[1] = 0x07;
    commands[2] = dev_strlen; // len

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    uint32_t bytes_to_copy = 8;
    if ( name_size < bytes_to_copy )
    {
        bytes_to_copy = name_size;
    }
    memcpy( name, response + 1, bytes_to_copy );

    return 0;
}

int
corsairlink_axi_vendor(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_size )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x03;
    commands[1] = 0x99;
    commands[2] = 0x00;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    uint32_t bytes_to_copy = 16;
    if ( name_size < bytes_to_copy )
    {
        bytes_to_copy = name_size;
    }
    memcpy( name, response + 2, bytes_to_copy );

    return 0;
}

int
corsairlink_axi_product(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_size )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x03;
    commands[1] = 0x9a;
    commands[2] = 0x00;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    uint32_t bytes_to_copy = 16;
    if ( name_size < bytes_to_copy )
    {
        bytes_to_copy = name_size;
    }
    memcpy( name, response + 2, bytes_to_copy );

    return 0;
}
