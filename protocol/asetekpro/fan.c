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
corsairlink_asetekpro_fan_count(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr = 0;
    // undefined, return device value from device.c
    ctrl->fan_count = dev->fan_control_count;
    return rr;
}

int
corsairlink_asetekpro_fan_print_mode(
    uint8_t mode, uint16_t data, char* modestr, uint8_t modestr_size )
{
    int rr = 0;
    // undefined, return hex value of mode
    snprintf( modestr, modestr_size, "Mode 0x%02X", mode );
    return rr;
}

int
corsairlink_asetekpro_fan_mode(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    if ( ctrl->mode == PERFORMANCE )
    {
        ASETEKPRO_FAN_TABLE_EXTREME( ctrl->table );
    }
    else if ( ctrl->mode == QUIET )
    {
        ASETEKPRO_FAN_TABLE_QUIET( ctrl->table );
    }
    rr = dev->driver->fan.profile.write_custom_curve( dev, handle, ctrl );

    return rr;
}

int
corsairlink_asetekpro_fan_mode_read(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x20;

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 32 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 32 );

    msg_debug2( "%02X\n", response[0x16] );
    ctrl->mode = response[0x16];

    return rr;
}

int
corsairlink_asetekpro_fan_mode_performance(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    ASETEKPRO_FAN_TABLE_EXTREME( ctrl->table );
    rr = dev->driver->fan.profile.write_custom_curve( dev, handle, ctrl );

    return rr;
}

int
corsairlink_asetekpro_fan_mode_balanced(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    ASETEKPRO_FAN_TABLE_BALANCED( ctrl->table );
    rr = dev->driver->fan.profile.write_custom_curve( dev, handle, ctrl );

    return rr;
}

int
corsairlink_asetekpro_fan_mode_quiet(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    ASETEKPRO_FAN_TABLE_QUIET( ctrl->table );
    rr = dev->driver->fan.profile.write_custom_curve( dev, handle, ctrl );

    return rr;
}

int
corsairlink_asetekpro_fan_curve(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );
    
    commands[0] = AsetekProFanWrite;
    commands[1] = ctrl->channel;

    commands[2] = ctrl->table[0].temperature;
    commands[3] = ctrl->table[1].temperature;
    commands[4] = ctrl->table[2].temperature;
    commands[5] = ctrl->table[3].temperature;
    commands[6] = ctrl->table[4].temperature;
    commands[7] = ctrl->table[5].temperature;
    commands[8] = ctrl->table[6].temperature;
    
    commands[9] = ctrl->table[0].speed;
    commands[10] = ctrl->table[1].speed;
    commands[11] = ctrl->table[2].speed;
    commands[12] = ctrl->table[3].speed;
    commands[13] = ctrl->table[4].speed;
    commands[14] = ctrl->table[5].speed;
    commands[15] = ctrl->table[6].speed;

    
    msg_debug2( "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X \n", commands[0], commands[1], commands[2], commands[3], commands[4], commands[5], commands[6], commands[7], commands[8], commands[9], commands[10], commands[11], commands[12], commands[13], commands[14], commands[15]);


    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 16 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 32 );

    return rr;
}

int
corsairlink_asetekpro_fan_speed(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = AsetekProFanRead; // fan speed query
    commands[1] = ctrl->channel; // fan port

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 6 );

    msg_debug2(
        "%02X %02X %02X %02X %02X %02X\n", response[0], response[1], response[2], response[3],
        response[4], response[5] );

    if ( response[0] != 0x41 || response[1] != 0x12 || response[2] != 0x34
         || response[3] != ctrl->channel )
    {
        msg_debug2( "Bad Response\n" );
    }

    ctrl->speed_rpm = ( response[4] << 8 ) + response[5];
    ctrl->max_speed = 0;

    return rr;
}


int
corsairlink_asetekpro_fan_mode_rpm(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = AsetekProFanFixedRPMWrite;
    commands[1] = ctrl->channel;
    commands[2] = ( ctrl->speed_rpm >> 8 );
    commands[3] = ctrl->speed_rpm;
    
    msg_debug2("%02X %02X %02X %02X\n", commands[0], commands[1], commands[2], commands[3]);

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 4 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 64 );

    return rr;
}

int
corsairlink_asetekpro_fan_mode_pwm(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = AsetekProFanFixedPWMWrite;
    commands[1] = ctrl->channel;
    commands[2] = ctrl->speed_pwm;
    
    msg_debug2("%02X %02X %02X\n", commands[0], commands[1], commands[2]);

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 3 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 32 );

    return rr;
}