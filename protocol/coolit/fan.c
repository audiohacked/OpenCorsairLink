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
corsairlink_coolit_fan_count(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;

    commands[++ii] = CommandId++;
    commands[++ii] = ReadOneByte;
    commands[++ii] = FAN_Count;

    commands[0] = ii;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
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
corsairlink_coolit_fan_mode_read_rpm(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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

    ii = 0;
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[++ii] = CommandId++;
    commands[++ii] = ReadTwoBytes;
    commands[++ii] = FAN_FixedRPM;
    commands[0] = ii;
    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
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

    ii = 0;
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[++ii] = CommandId++;
    commands[++ii] = ReadOneByte;
    commands[++ii] = FAN_FixedPWM;
    commands[0] = ii;
    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
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
corsairlink_coolit_fan_mode_balanced(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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
corsairlink_coolit_fan_mode_quiet(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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

    commands[++ii] = COOLIT_Quiet;

    commands[0] = ii;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
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
corsairlink_coolit_fan_mode_rpm(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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

    commands[++ii] = COOLIT_FixedRPM;
    commands[++ii] = CommandId++;
    commands[++ii] = WriteTwoBytes;
    commands[++ii] = FAN_FixedRPM;
    commands[++ii] = ctrl->speed_rpm & 0xFF;
    commands[++ii] = ( ctrl->speed_rpm >> 8 ) & 0xFF;

    commands[0] = ii;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
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

    uint8_t ii = 0;
    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Select;
    commands[++ii] = ctrl->channel;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Mode;

    commands[++ii] = COOLIT_FixedPWM;
    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_FixedPWM;
    commands[++ii] = ctrl->speed_pwm & 0xFF;

    commands[0] = ii;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
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
corsairlink_coolit_fan_curve(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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
corsairlink_coolit_fan_speed(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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
    ctrl->speed_rpm = ( response[5] << 8 ) + response[4];
    ctrl->max_speed = ( response[9] << 8 ) + response[8];

    return rr;
}
