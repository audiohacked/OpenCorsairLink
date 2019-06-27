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
#include "lowlevel/commanderpro.h"
#include "print.h"
#include "protocol/commanderpro.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_commanderpro_fan_count(
        struct corsair_device_info* dev,
        struct libusb_device_handle* handle,
        struct fan_control* ctrl )
{
    int rr = 0;
    // undefined, return device value from device.c
    ctrl->fan_count = dev->fan_control_count;
    return rr;
}

int
corsairlink_commanderpro_fan_mode_read(
        struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x29;
    commands[1] = 0x01;
    commands[2] = (uint8_t) ctrl->channel;

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 16 );

    dump_packet( commands, sizeof( commands ) );
    dump_packet( response, sizeof( response ) );
    //msg_debug2( "%02X\n", response[ctrl->channel] );
    if ( response[2] == ctrl->channel )
    {
        ctrl->mode = response[3];
    }
    else
    {
        ctrl->mode = 0x03;
    }

    return rr;
}

int
corsairlink_commanderpro_fan_print_mode(
    uint8_t mode, uint16_t data, char* modestr, uint8_t modestr_size )
{
    int rr = 0;
    *modestr = '\0';
    switch ( mode )
    {
        case 0x00:
            strncat(modestr, "Mode: Auto/Disconnected", modestr_size);
            break;
        case 0x01:
            strncat(modestr, "Mode: 3-Pin", modestr_size);
            break;
        case 0x02:
            strncat(modestr, "Mode: 4-Pin", modestr_size);
            break;
        case 0x03:
            strncat(modestr, "Mode: Unknown", modestr_size);
            break;
        default:
            snprintf( modestr, modestr_size, "Mode: 0x%02X", mode );
            break;
    }

    return rr;
}

int
corsairlink_commanderpro_get_fan_setup_mask(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x20;
    // commands[1] = (uint8_t) ctrl->channel;

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 16 );

    dump_packet( commands, sizeof( commands ) );
    dump_packet( response, sizeof( response ) );
    for ( int ii = 1; ii <= 6; ++ii )
    {
        switch ( response[ii] )
        {
        case 0x00:
            msg_debug2( "Auto/Disconnected" );
            break;
        case 0x01:
            msg_debug2( "3-Pin" );
            break;
        case 0x02:
            msg_debug2( "4-Pin" );
            break;
        case 0x03:
            msg_debug2( "Unknown" );
            break;
        default:
            msg_debug2( "Impossible!" );
        }
    }

    return rr;
}

int
corsairlink_commanderpro_get_fan_speed_rpm(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x21;
    commands[1] = (uint8_t) ctrl->channel;

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 16 );

    ctrl->speed_rpm = ( response[1] << 8 ) + response[2];

    dump_packet( commands, sizeof( commands ) );
    dump_packet( response, sizeof( response ) );
    return rr;
}

int
corsairlink_commanderpro_get_fan_speed_pwm(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x22;
    commands[1] = (uint8_t) ctrl->channel;

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 16 );

    ctrl->speed_pwm = response[1];

    dump_packet( commands, sizeof( commands ) );
    dump_packet( response, sizeof( response ) );
    return rr;
}

int
corsairlink_commanderpro_set_fan_speed_pwm(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x23;
    commands[1] = (uint8_t) ctrl->channel;
    commands[2] = ctrl->speed_pwm;

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 16 );

    dump_packet( commands, sizeof( commands ) );
    dump_packet( response, sizeof( response ) );
    return rr;
}

int
corsairlink_commanderpro_set_fan_speed_rpm(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x24;
    commands[1] = (uint8_t) ctrl->channel;
    commands[2] = (uint8_t) (ctrl->speed_rpm >> 8);
    commands[3] = (uint8_t) (ctrl->speed_rpm & 0xff);

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 16 );

    dump_packet( commands, sizeof( commands ) );
    dump_packet( response, sizeof( response ) );
    return rr;
}

int
corsairlink_commanderpro_get_fan_detect_type(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x29;
    commands[1] = 0x01;
    commands[2] = (uint8_t) ctrl->channel;

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 16 );

    dump_packet( commands, sizeof( commands ) );
    dump_packet( response, sizeof( response ) );
    return rr;
}

int
corsairlink_commanderpro_set_fan_curve(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x25;
    commands[1] = (uint8_t) ctrl->channel;
    commands[2] = 0x00; // 0x00 = CP Temp Probe 1 .... 0x03 = CP Temp Probe 4,
                        // 0xff = External

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 16 );

    dump_packet( commands, sizeof( commands ) );
    dump_packet( response, sizeof( response ) );
    return rr;
}

int
corsairlink_commanderpro_send_temperature_info(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t sensor_index,
    uint8_t temperature )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x26;
    commands[1] = sensor_index;
    commands[2] = temperature >> 8;
    commands[3] = temperature & 0xff;

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 16 );

    dump_packet( commands, sizeof( commands ) );
    dump_packet( response, sizeof( response ) );
    return rr;
}

int
corsairlink_commanderpro_set_fan_connection_mode(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x28;
    commands[1] = 0x02;
    commands[2] = (uint8_t) ctrl->channel;
    // switch(response[ii])
    // {
    // case 0x00:
    //     msg_debug2("Auto/Disconnected");
    //     break;
    // case 0x01:
    //     msg_debug2("3-Pin");
    //     break;
    // case 0x02:
    //     msg_debug2("4-Pin");
    //     break;
    // case 0x03:
    //     msg_debug2("Unknown");
    //     break;
    // default:
    //     msg_debug2("Impossible!");
    // }
    commands[3] = ctrl->fan_type;

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 16 );

    dump_packet( commands, sizeof( commands ) );
    dump_packet( response, sizeof( response ) );
    return rr;
}
