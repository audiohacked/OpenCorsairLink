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
#include "lowlevel/coolit.h"
#include "print.h"
#include "protocol/coolit.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_coolit_pump_mode(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];

    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Select;
    commands[++ii] = ctrl->channel;

    commands[++ii] = CommandId++;
    commands[++ii] = ReadOneByte;
    commands[++ii] = FAN_Mode;

    commands[0] = ii;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    ctrl->mode = response[4];

    return rr;
}

int
corsairlink_coolit_pump_mode_read(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];

    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Select;
    commands[++ii] = ctrl->channel;

    commands[++ii] = CommandId++;
    commands[++ii] = ReadOneByte;
    commands[++ii] = FAN_Mode;

    commands[0] = ii;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    ctrl->mode = response[4];

    return rr;
}

int
corsairlink_coolit_pump_mode_quiet(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];

    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Select;
    commands[++ii] = ctrl->channel;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Mode;

    /*if ( dev->device_id == CoolitH110i
         || dev->device_id == CoolitH110iGT ) // both only accept fixed RPMs
    {
        commands[++ii] = COOLIT_FixedRPM;
        commands[++ii] = CommandId++;
        commands[++ii] = WriteTwoBytes;
        commands[++ii] = FAN_FixedRPM;
        commands[++ii] = new_pump_mode == PERFORMANCE ? 0x86 : 0x2E;
        commands[++ii] = new_pump_mode == PERFORMANCE ? 0x0B : 0x09;
    }*/

    commands[++ii] = COOLIT_Quiet;
    commands[0] = ii;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    return rr;
}

int
corsairlink_coolit_pump_mode_balanced(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];

    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Select;
    commands[++ii] = ctrl->channel;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Mode;

    commands[++ii] = COOLIT_Balanced;
    commands[0] = ii;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    return rr;
}

int
corsairlink_coolit_pump_mode_performance(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];

    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Select;
    commands[++ii] = ctrl->channel;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Mode;
    commands[++ii] = COOLIT_Performance;

    commands[0] = ii;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    return rr;
}

int
corsairlink_coolit_pump_mode_default(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];

    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Select;
    commands[++ii] = ctrl->channel;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Mode;
    commands[++ii] = COOLIT_Default;

    commands[0] = ii;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    return rr;
}

int
corsairlink_coolit_pump_mode_custom(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;
    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Select;
    commands[++ii] = ctrl->channel;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Mode;

    commands[++ii] = COOLIT_Custom;

    commands[0] = ii;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    return rr;
}

int
corsairlink_coolit_pump_curve(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    // commands[0] = FanCurve;
    // commands[1] = UnknownFanCurve;

    uint8_t ii = 0;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteThreeBytes;
    commands[++ii] = FAN_TempTable;
    commands[++ii] = 0x0A;

    commands[++ii] = ctrl->table[0].temperature;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[1].temperature;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[2].temperature;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[3].temperature;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[4].temperature;
    commands[++ii] = 0x00;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteThreeBytes;
    commands[++ii] = FAN_RPMTable;
    commands[++ii] = 0x0A;

    commands[++ii] = ctrl->table[0].speed;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[1].speed;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[2].speed;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[3].speed;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[4].speed;
    commands[++ii] = 0x00;

    commands[0] = ii;
    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    return rr;
}

int
corsairlink_coolit_pump_speed(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Select;
    commands[++ii] = ctrl->channel;

    commands[++ii] = CommandId++;
    commands[++ii] = ReadTwoBytes;
    commands[++ii] = FAN_ReadRPM;

    commands[++ii] = CommandId++;
    commands[++ii] = ReadTwoBytes;
    commands[++ii] = FAN_MaxRecordedRPM;

    commands[0] = ii;
    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    msg_debug2( "Speed: %02X %02X\n", response[5], response[4] );
    msg_debug2( "Max Speed: %02X %02X\n", response[9], response[8] );
    ctrl->speed = ( response[5] << 8 ) + response[4];
    ctrl->max_speed = ( response[9] << 8 ) + response[8];

    return rr;
}
