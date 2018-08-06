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
#include "print.h"
#include "protocol/commanderpro.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_commanderpro_fan_print_mode(
    uint8_t mode, uint16_t data, char* modestr, uint8_t modestr_size )
{
    int rr = 0;
    // undefined, return hex value of mode
    snprintf( modestr, modestr_size, "Mode 0x%02X", mode );
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
    commands[1] = ctrl->channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 14 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 32 );

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

    ctrl->fan_type = response[ctrl->channel + 1];

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
    commands[1] = ctrl->channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 14 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 32 );

    ctrl->speed_rpm = ( response[1] << 8 ) + response[2];

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
    commands[1] = ctrl->channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 14 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 32 );

    ctrl->speed_pwm = response[1];

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
    commands[1] = ctrl->channel;
    commands[2] = ctrl->speed_pwm;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 14 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 32 );

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
    commands[1] = ctrl->channel;
    commands[2] = ctrl->speed_rpm >> 8;
    commands[3] = ctrl->speed_rpm & 0xff;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 14 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 32 );

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
    commands[1] = ctrl->channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 14 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 32 );

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
    commands[1] = ctrl->channel;
    commands[2] = 0x00; // 0x00 = CP Temp Probe 1 .... 0x03 = CP Temp Probe 4,
                        // 0xff = External
    commands[3] = ctrl->table[0].temperature >> 8;
    commands[4] = ctrl->table[0].temperature & 0xFF;

    commands[5] = ctrl->table[1].temperature >> 8;
    commands[6] = ctrl->table[1].temperature & 0xFF;

    commands[7] = ctrl->table[2].temperature >> 8;
    commands[8] = ctrl->table[2].temperature & 0xFF;

    commands[9] = ctrl->table[3].temperature >> 8;
    commands[10] = ctrl->table[3].temperature & 0xFF;

    commands[11] = ctrl->table[4].temperature >> 8;
    commands[12] = ctrl->table[4].temperature & 0xFF;

    commands[13] = ctrl->table[5].temperature >> 8;
    commands[14] = ctrl->table[5].temperature & 0xFF;

    commands[15] = ctrl->table[0].speed >> 8;
    commands[16] = ctrl->table[0].speed & 0xFF;

    commands[17] = ctrl->table[1].speed >> 8;
    commands[18] = ctrl->table[1].speed & 0xFF;

    commands[19] = ctrl->table[2].speed >> 8;
    commands[20] = ctrl->table[2].speed & 0xFF;

    commands[21] = ctrl->table[3].speed >> 8;
    commands[22] = ctrl->table[3].speed & 0xFF;

    commands[23] = ctrl->table[4].speed >> 8;
    commands[24] = ctrl->table[4].speed & 0xFF;

    commands[25] = ctrl->table[5].speed >> 8;
    commands[26] = ctrl->table[5].speed & 0xFF;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 32 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 32 );

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

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 14 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 32 );

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
    commands[2] = ctrl->channel;
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

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 14 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 32 );

    return rr;
}
