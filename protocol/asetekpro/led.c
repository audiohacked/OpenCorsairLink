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
corsairlink_asetekpro_led_static_color(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct led_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    for ( int ii = 0; ii < 7; ++ii )
    {
        msg_debug2(
            "DEBUG: Color%d %02x%02x%02x\n", ii, ctrl->led_colors[ii].red,
            ctrl->led_colors[ii].green, ctrl->led_colors[ii].blue );
    }

    commands[0] = 0x56;
    commands[1] = 0x02;

    commands[2] = ctrl->led_colors[0].red;
    commands[3] = ctrl->led_colors[0].green;
    commands[4] = ctrl->led_colors[0].blue;

    commands[5] = ctrl->led_colors[0].red;
    commands[6] = ctrl->led_colors[0].green;
    commands[7] = ctrl->led_colors[0].blue;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 8 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 3 );

    msg_debug2( "%02X %02X %02X\n", response[0], response[1], response[2] );

    if ( response[0] != 0x56 || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    commands[0] = 0x55;
    commands[1] = 0x01;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 3 );

    msg_debug2( "%02X %02X %02X\n", response[0], response[1], response[2] );

    if ( response[0] != 0x55 || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    return rr;
}

int
corsairlink_asetekpro_led_blink(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct led_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x56;
    commands[1] = ctrl->count;

    commands[2] = ctrl->led_colors[0].red;
    commands[3] = ctrl->led_colors[0].green;
    commands[4] = ctrl->led_colors[0].blue;

    commands[5] = ctrl->led_colors[1].red;
    commands[6] = ctrl->led_colors[1].green;
    commands[7] = ctrl->led_colors[1].blue;

    commands[8] = ctrl->led_colors[2].red;
    commands[9] = ctrl->led_colors[2].green;
    commands[10] = ctrl->led_colors[2].blue;

    commands[11] = ctrl->led_colors[3].red;
    commands[12] = ctrl->led_colors[3].green;
    commands[13] = ctrl->led_colors[3].blue;

    commands[14] = ctrl->led_colors[4].red;
    commands[15] = ctrl->led_colors[4].green;
    commands[16] = ctrl->led_colors[4].blue;

    commands[17] = ctrl->led_colors[5].red;
    commands[18] = ctrl->led_colors[5].green;
    commands[19] = ctrl->led_colors[5].blue;

    commands[20] = ctrl->led_colors[6].red;
    commands[21] = ctrl->led_colors[6].green;
    commands[22] = ctrl->led_colors[6].blue;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 23 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 3 );

    msg_debug2( "%02X %02X %02X\n", response[0], response[1], response[2] );

    if ( response[0] != 0x56 || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    commands[0] = 0x53;
    switch ( ctrl->speed )
    {
    case AsetekProSpeedSlow:
        commands[1] = 0x0F;
        break;
    case AsetekProSpeedMedium:
        commands[1] = 0x0A;
        break;
    case AsetekProSpeedFast:
    default:
        commands[1] = 0x05;
    }

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 3 );

    msg_debug2( "%02X %02X %02X\n", response[0], response[1], response[2] );

    if ( response[0] != 0x53 || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    commands[0] = 0x58;
    commands[1] = 0x01;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 3 );

    msg_debug2( "%02X %02X %02X\n", response[0], response[1], response[2] );

    if ( response[0] != 0x58 || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    return rr;
}

int
corsairlink_asetekpro_led_color_pulse(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct led_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    /* colors */
    commands[0] = 0x56;
    commands[1] = ctrl->count;

    commands[2] = ctrl->led_colors[0].red;
    commands[3] = ctrl->led_colors[0].green;
    commands[4] = ctrl->led_colors[0].blue;

    commands[5] = ctrl->led_colors[1].red;
    commands[6] = ctrl->led_colors[1].green;
    commands[7] = ctrl->led_colors[1].blue;

    commands[8] = ctrl->led_colors[2].red;
    commands[9] = ctrl->led_colors[2].green;
    commands[10] = ctrl->led_colors[2].blue;

    commands[11] = ctrl->led_colors[3].red;
    commands[12] = ctrl->led_colors[3].green;
    commands[13] = ctrl->led_colors[3].blue;

    commands[14] = ctrl->led_colors[4].red;
    commands[15] = ctrl->led_colors[4].green;
    commands[16] = ctrl->led_colors[4].blue;

    commands[17] = ctrl->led_colors[5].red;
    commands[18] = ctrl->led_colors[5].green;
    commands[19] = ctrl->led_colors[5].blue;

    commands[20] = ctrl->led_colors[6].red;
    commands[21] = ctrl->led_colors[6].green;
    commands[22] = ctrl->led_colors[6].blue;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 23 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 3 );

    msg_debug2( "%02X %02X %02X\n", response[0], response[1], response[2] );

    if ( response[0] != 0x56 || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    /* Mode */
    commands[0] = 0x53;
    switch ( ctrl->speed )
    {
    case AsetekProSpeedSlow:
        commands[1] = 0x50;
        break;
    case AsetekProSpeedMedium:
        commands[1] = 0x37;
        break;
    case AsetekProSpeedFast:
    default:
        commands[1] = 0x1E;
    }

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 3 );

    msg_debug2( "%02X %02X %02X\n", response[0], response[1], response[2] );

    if ( response[0] != 0x53 || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    /* Commit */
    commands[0] = 0x52;
    commands[1] = 0x01;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 3 );

    msg_debug2( "%02X %02X %02X\n", response[0], response[1], response[2] );

    if ( response[0] != 0x52 || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    return rr;
}

int
corsairlink_asetekpro_led_color_shift(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct led_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x56;
    commands[1] = ctrl->count;

    commands[2] = ctrl->led_colors[0].red;
    commands[3] = ctrl->led_colors[0].green;
    commands[4] = ctrl->led_colors[0].blue;

    commands[5] = ctrl->led_colors[1].red;
    commands[6] = ctrl->led_colors[1].green;
    commands[7] = ctrl->led_colors[1].blue;

    commands[8] = ctrl->led_colors[2].red;
    commands[9] = ctrl->led_colors[2].green;
    commands[10] = ctrl->led_colors[2].blue;

    commands[11] = ctrl->led_colors[3].red;
    commands[12] = ctrl->led_colors[3].green;
    commands[13] = ctrl->led_colors[3].blue;

    commands[14] = ctrl->led_colors[4].red;
    commands[15] = ctrl->led_colors[4].green;
    commands[16] = ctrl->led_colors[4].blue;

    commands[17] = ctrl->led_colors[5].red;
    commands[18] = ctrl->led_colors[5].green;
    commands[19] = ctrl->led_colors[5].blue;

    commands[20] = ctrl->led_colors[6].red;
    commands[21] = ctrl->led_colors[6].green;
    commands[22] = ctrl->led_colors[6].blue;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 23 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 3 );

    msg_debug2( "%02X %02X %02X\n", response[0], response[1], response[2] );

    if ( response[0] != 0x56 || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    commands[0] = 0x53;
    switch ( ctrl->speed )
    {
    case AsetekProSpeedSlow:
        commands[1] = 0x46;
        break;
    case AsetekProSpeedMedium:
        commands[1] = 0x28;
        break;
    case AsetekProSpeedFast:
    default:
        commands[1] = 0x0F;
    }

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 3 );

    msg_debug2( "%02X %02X %02X\n", response[0], response[1], response[2] );

    if ( response[0] != 0x53 || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    commands[0] = 0x55;
    commands[1] = 0x01;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 3 );

    msg_debug2( "%02X %02X %02X\n", response[0], response[1], response[2] );

    if ( response[0] != 0x55 || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    return rr;
}

int
corsairlink_asetekpro_led_rainbow(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct led_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x53;
    switch ( ctrl->speed )
    {
    case AsetekProSpeedSlow:
        commands[1] = 0x30;
        break;
    case AsetekProSpeedMedium:
        commands[1] = 0x18;
        break;
    case AsetekProSpeedFast:
    default:
        commands[1] = 0x0C;
    }

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 3 );

    msg_debug2(
        "DEBUG: Rainbow mode response %02X %02X %02X\n", response[0], response[1], response[2] );

    if ( response[0] != 0x53 || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    commands[0] = 0x55;
    commands[1] = 0x01;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 3 );

    msg_debug2(
        "DEBUG: Rainbow commit response %02X %02X %02X\n", response[0], response[1], response[2] );

    if ( response[0] != 0x55 || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    return rr;
}

int
corsairlink_asetekpro_led_temperature(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct led_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x5f;
    commands[1] = ctrl->temperatures[0];
    commands[2] = 0x00;
    commands[3] = ctrl->temperatures[1];
    commands[4] = 0x00;
    commands[5] = ctrl->temperatures[2];
    commands[6] = 0x00;
    commands[7] = ctrl->led_colors[0].red;
    commands[8] = ctrl->led_colors[0].green;
    commands[9] = ctrl->led_colors[0].blue;
    commands[10] = ctrl->led_colors[1].red;
    commands[11] = ctrl->led_colors[1].green;
    commands[12] = ctrl->led_colors[1].blue;
    commands[13] = ctrl->led_colors[2].red;
    commands[14] = ctrl->led_colors[2].green;
    commands[15] = ctrl->led_colors[2].blue;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 16 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 6 );

    msg_debug2(
        "%02X %02X %02X %02X %02X %02X\n", response[0], response[1], response[2], response[3],
        response[4], response[5] );

    if ( response[0] != 0x5f || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    commands[0] = 0x5E;
    commands[1] = 0x01;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 3 );

    msg_debug2( "%02X %02X %02X\n", response[0], response[1], response[2] );

    if ( response[0] != 0x5E || response[1] != 0x12 || response[2] != 0x34 )
    {
        msg_debug2( "Bad Response\n" );
    }

    return rr;
}
