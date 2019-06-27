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
corsairlink_coolit_fan_count(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;

    commands[0] = ReadOneByte;
    commands[1] = FAN_Count;

    commands[0] = ii;

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 64 );
    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    ctrl->fan_count = response[2] - 1; // we subtract 1 because count includes pump

    return rr;
}

int
corsairlink_coolit_fan_print_mode(
    uint8_t mode, uint16_t data, char* modestr, uint8_t modestr_size )
{
    int rr = 0;
    uint8_t isConnected = mode & 0x80;
    uint8_t is4pin = mode & 0x01;
    uint8_t real_mode = mode & 0x0E;

    if ( !isConnected )
        snprintf( modestr, modestr_size, "Not connected or failed" );
    else if ( real_mode == COOLIT_Performance )
        snprintf( modestr, modestr_size, "Performance Mode (%s)", is4pin ? "4PIN" : "3PIN" );
    else if ( real_mode == COOLIT_Balanced )
        snprintf( modestr, modestr_size, "Balanced Mode (%s)", is4pin ? "4PIN" : "3PIN" );
    else if ( real_mode == COOLIT_Quiet )
        snprintf( modestr, modestr_size, "Quiet Mode (%s)", is4pin ? "4PIN" : "3PIN" );
    else if ( real_mode == COOLIT_Default )
        snprintf( modestr, modestr_size, "Default Mode (%s)", is4pin ? "4PIN" : "3PIN" );
    else if ( real_mode == COOLIT_FixedPWM )
        snprintf(
            modestr, modestr_size, "Fixed PWM Mode (%s) set to %d%%", is4pin ? "4PIN" : "3PIN",
            ( data + 1 ) * 100 / 256 );
    else if ( real_mode == COOLIT_FixedRPM )
        snprintf(
            modestr, modestr_size, "Fixed RPM Mode (%s) set to %d", is4pin ? "4PIN" : "3PIN",
            data );
    else if ( real_mode == COOLIT_Custom )
        snprintf( modestr, modestr_size, "Custom Curve Mode (%s)", is4pin ? "4PIN" : "3PIN" );
    return rr;
}

int
corsairlink_coolit_fan_mode_read(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 64 );
    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    ctrl->mode = response[4];

    return rr;
}

int
corsairlink_coolit_fan_mode_read_rpm(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 64 );
    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    ctrl->mode = response[4];

    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[++ii] = CommandId++;
    commands[++ii] = ReadTwoBytes;
    commands[++ii] = FAN_FixedRPM;
    commands[0] = ii;
    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 64 );
    commands[0] = ReadTwoBytes;
    commands[1] = FAN_FixedRPM;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );
    ctrl->speed_rpm = ( response[3] << 8 ) + response[2];

    return rr;
}

int
corsairlink_coolit_fan_mode_read_pwm(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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

    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = ReadOneByte;
    commands[1] = FAN_FixedPWM;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    ctrl->speed_pwm = response[2];

    return rr;
}

int
corsairlink_coolit_fan_mode_performance(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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
corsairlink_coolit_fan_mode_balanced(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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
corsairlink_coolit_fan_mode_quiet(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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
    commands[2] = COOLIT_Quiet;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    return rr;
}

int
corsairlink_coolit_fan_mode_default(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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
corsairlink_coolit_fan_mode_rpm(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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
    commands[2] = COOLIT_FixedRPM;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );

    commands[0] = WriteTwoBytes;
    commands[1] = FAN_FixedRPM;
    commands[2] = ctrl->speed_rpm & 0xFF;
    commands[3] = ( ctrl->speed_rpm >> 8 ) & 0xFF;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 4 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    return rr;
}

int
corsairlink_coolit_fan_mode_pwm(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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
    commands[2] = COOLIT_FixedPWM;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );

    commands[0] = WriteOneByte;
    commands[1] = FAN_FixedPWM;
    commands[2] = ctrl->speed_pwm & 0xFF;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    return rr;
}

int
corsairlink_coolit_fan_mode_custom(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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
corsairlink_coolit_fan_curve(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

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
corsairlink_coolit_fan_speed(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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
    ctrl->speed_rpm = ( response[5] << 8 ) + response[4];

    commands[0] = ReadTwoBytes;
    commands[1] = FAN_MaxRecordedRPM;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 2 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    msg_debug2( "Max Speed: %02X %02X\n", response[5], response[4] );
    ctrl->max_speed = ( response[5] << 8 ) + response[4];

    return rr;
}
