/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017-2020  Sean Nelson <audiohacked@gmail.com>

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
#include "lowlevel/coolit.h"
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

    commands[0] = WriteOneByte;
    commands[1] = FAN_Select;
    commands[2] = ctrl->channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );

    commands[0] = ReadOneByte;
    commands[1] = FAN_Mode;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
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

    commands[0] = WriteOneByte;
    commands[1] = FAN_Select;
    commands[2] = ctrl->channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );

    commands[0] = ReadOneByte;
    commands[1] = FAN_Mode;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
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

    commands[0] = WriteOneByte;
    commands[1] = FAN_Select;
    commands[2] = ctrl->channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );

    commands[0] = WriteOneByte;
    commands[1] = FAN_Mode;

    /*
    if ( dev->device_id == CoolitH110i
         || dev->device_id == CoolitH110iGT ) // both only accept fixed RPMs
    {
        commands[0] = COOLIT_FixedRPM;
        commands[0] = CommandId++;
        commands[0] = WriteTwoBytes;
        commands[0] = FAN_FixedRPM;
        commands[0] = new_pump_mode == PERFORMANCE ? 0x86 : 0x2E;
        commands[0] = new_pump_mode == PERFORMANCE ? 0x0B : 0x09;
    }
    */

    commands[2] = COOLIT_Quiet;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );
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

    commands[0] = WriteOneByte;
    commands[1] = FAN_Select;
    commands[2] = ctrl->channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );

    commands[0] = WriteOneByte;
    commands[1] = FAN_Mode;
    commands[2] = COOLIT_Balanced;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );
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

    commands[0] = WriteOneByte;
    commands[1] = FAN_Select;
    commands[2] = ctrl->channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );

    commands[0] = WriteOneByte;
    commands[1] = FAN_Mode;
    commands[2] = COOLIT_Performance;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );
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

    commands[0] = WriteOneByte;
    commands[1] = FAN_Select;
    commands[2] = ctrl->channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );

    commands[0] = WriteOneByte;
    commands[1] = FAN_Mode;
    commands[2] = COOLIT_Default;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );
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

    commands[0] = WriteOneByte;
    commands[1] = FAN_Select;
    commands[2] = ctrl->channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );

    commands[0] = WriteOneByte;
    commands[1] = FAN_Mode;
    commands[2] = COOLIT_Custom;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );
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

    commands[0] = WriteThreeBytes;
    commands[1] = FAN_TempTable;
    commands[2] = 0x0A;

    commands[3] = ctrl->table[0].temperature;
    commands[4] = 0x00;
    commands[5] = ctrl->table[1].temperature;
    commands[6] = 0x00;
    commands[7] = ctrl->table[2].temperature;
    commands[8] = 0x00;
    commands[9] = ctrl->table[3].temperature;
    commands[10] = 0x00;
    commands[11] = ctrl->table[4].temperature;
    commands[12] = 0x00;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 13 );

    commands[0] = WriteThreeBytes;
    commands[1] = FAN_RPMTable;
    commands[2] = 0x0A;

    commands[3] = ctrl->table[0].speed;
    commands[4] = 0x00;
    commands[5] = ctrl->table[1].speed;
    commands[6] = 0x00;
    commands[7] = ctrl->table[2].speed;
    commands[8] = 0x00;
    commands[9] = ctrl->table[3].speed;
    commands[10] = 0x00;
    commands[11] = ctrl->table[4].speed;
    commands[12] = 0x00;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 13 );
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

    commands[0] = WriteOneByte;
    commands[1] = FAN_Select;
    commands[2] = ctrl->channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );

    commands[0] = ReadTwoBytes;
    commands[1] = FAN_ReadRPM;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    msg_debug2( "Speed: %02X %02X\n", response[5], response[4] );
    ctrl->speed = ( response[5] << 8 ) + response[4];

    commands[0] = ReadTwoBytes;
    commands[1] = FAN_MaxRecordedRPM;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    msg_debug2( "Max Speed: %02X %02X\n", response[5], response[4] );
    ctrl->max_speed = ( response[5] << 8 ) + response[4];

    return rr;
}
