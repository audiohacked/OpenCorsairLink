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
#include "protocol/asetek.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_asetek_fan_count(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr = 0;
    // undefined, return device value from device.c
    ctrl->fan_count = dev->fan_control_count;
    return rr;
}

int
corsairlink_asetek_fan_print_mode(
    uint8_t mode, uint16_t data, char* modestr, uint8_t modestr_size )
{
    int rr = 0;
    // undefined, return hex value of mode
    snprintf( modestr, modestr_size, "Mode 0x%02X", mode );
    return rr;
}

int
corsairlink_asetek_fan_mode(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    if ( ctrl->mode == PERFORMANCE )
    {
        ASETEK_FAN_TABLE_EXTREME( ctrl->table );
    }
    else if ( ctrl->mode == QUIET )
    {
        ASETEK_FAN_TABLE_QUIET( ctrl->table );
    }
    rr = dev->driver->fan.profile.write_custom_curve( dev, handle, ctrl );

    return rr;
}

int
corsairlink_asetek_fan_mode_read(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x20;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 32 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 32 );

    msg_debug2( "%02X\n", response[0x16] );
    ctrl->mode = response[0x16];

    return rr;
}

int
corsairlink_asetek_fan_mode_performance(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    ASETEK_FAN_TABLE_EXTREME( ctrl->table );
    rr = dev->driver->fan.profile.write_custom_curve( dev, handle, ctrl );

    return rr;
}

int
corsairlink_asetek_fan_mode_balanced(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    ASETEK_FAN_TABLE_BALANCED( ctrl->table );
    rr = dev->driver->fan.profile.write_custom_curve( dev, handle, ctrl );

    return rr;
}

int
corsairlink_asetek_fan_mode_quiet(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    ASETEK_FAN_TABLE_QUIET( ctrl->table );
    rr = dev->driver->fan.profile.write_custom_curve( dev, handle, ctrl );

    return rr;
}

int
corsairlink_asetek_fan_curve(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );
    
    commands[0] = 0x40;
    commands[1] = ctrl->channel;

    commands[2] = 0;
    commands[3] = ctrl->table[0].temperature;
    commands[4] = ctrl->table[1].temperature;
    commands[5] = ctrl->table[2].temperature;
    commands[6] = ctrl->table[3].temperature;
    commands[7] = ctrl->table[4].temperature;
    commands[8] = ctrl->table[5].temperature;
    
    commands[9] = 0;
    commands[10] = ctrl->table[0].speed;
    commands[11] = ctrl->table[1].speed;
    commands[12] = ctrl->table[2].speed;
    commands[13] = ctrl->table[3].speed;
    commands[14] = ctrl->table[4].speed;
    commands[15] = ctrl->table[5].speed;

    
    msg_debug2( "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X \n", commands[0], commands[1], commands[2], commands[3], commands[4], commands[5], commands[6], commands[7], commands[8], commands[9], commands[10], commands[11], commands[12], commands[13], commands[14], commands[15]);


    rr = dev->driver->write( handle, dev->write_endpoint, commands, 16 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 32 );

    return rr;
}

int
corsairlink_asetek_fan_speed(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x20;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 32 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 32 );

    msg_debug2( "%02X %02X\n", response[0], response[1] );
    ctrl->speed = ( response[0] << 8 ) + response[1];

    return rr;
}
