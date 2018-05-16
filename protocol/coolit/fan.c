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

int corsairlink_coolit_fan_count( struct corsair_device_info* dev,
                                  struct libusb_device_handle* handle,
                                  uint8_t* fan_count )
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

    *( fan_count ) =
        response[2] - 1; // we subtract 1 because count includes pump

    return rr;
}

int corsairlink_coolit_fan_print_mode( uint8_t mode, uint16_t data,
                                       char* modestr, uint8_t modestr_size )
{
    int rr = 0;
    uint8_t isConnected = mode & 0x80;
    uint8_t is4pin = mode & 0x01;
    uint8_t real_mode = mode & 0x0E;

    if ( !isConnected )
        snprintf( modestr, modestr_size, "Not connected or failed" );
    else if ( real_mode == COOLIT_Performance )
        snprintf( modestr,
                  modestr_size,
                  "Performance Mode (%s)",
                  is4pin ? "4PIN" : "3PIN" );
    else if ( real_mode == COOLIT_Balanced )
        snprintf( modestr,
                  modestr_size,
                  "Balanced Mode (%s)",
                  is4pin ? "4PIN" : "3PIN" );
    else if ( real_mode == COOLIT_Quiet )
        snprintf( modestr,
                  modestr_size,
                  "Quiet Mode (%s)",
                  is4pin ? "4PIN" : "3PIN" );
    else if ( real_mode == COOLIT_Default )
        snprintf( modestr,
                  modestr_size,
                  "Default Mode (%s)",
                  is4pin ? "4PIN" : "3PIN" );
    else if ( real_mode == COOLIT_FixedPWM )
        snprintf( modestr,
                  modestr_size,
                  "Fixed PWM Mode (%s) set to %d%%",
                  is4pin ? "4PIN" : "3PIN",
                  ( data + 1 ) * 100 / 256 );
    else if ( real_mode == COOLIT_FixedRPM )
        snprintf( modestr,
                  modestr_size,
                  "Fixed RPM Mode (%s) set to %d",
                  is4pin ? "4PIN" : "3PIN",
                  data );
    else if ( real_mode == COOLIT_Custom )
        snprintf( modestr,
                  modestr_size,
                  "Custom Curve Mode (%s)",
                  is4pin ? "4PIN" : "3PIN" );
    return rr;
}

int corsairlink_coolit_fan_mode( struct corsair_device_info* dev,
                                 struct libusb_device_handle* handle,
                                 uint8_t selector, uint8_t* fan_mode,
                                 uint16_t* fan_data )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t new_fan_mode = *( fan_mode );

    uint8_t ii = 0;
    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Select;
    commands[++ii] = selector;

    if ( new_fan_mode == UNDEFINED )
    {
        commands[++ii] = CommandId++;
        commands[++ii] = ReadOneByte;
        commands[++ii] = FAN_Mode;
    }
    else
    {
        commands[++ii] = CommandId++;
        commands[++ii] = WriteOneByte;
        commands[++ii] = FAN_Mode;

        if ( new_fan_mode == PERFORMANCE )
            commands[++ii] = COOLIT_Performance;
        else if ( new_fan_mode == BALANCED )
            commands[++ii] = COOLIT_Balanced;
        else if ( new_fan_mode == QUIET )
            commands[++ii] = COOLIT_Quiet;
        else if ( new_fan_mode == DEFAULT )
            commands[++ii] = COOLIT_Default;
        else if ( new_fan_mode == RPM )
        {
            commands[++ii] = COOLIT_FixedRPM;
            commands[++ii] = CommandId++;
            commands[++ii] = WriteTwoBytes;
            commands[++ii] = FAN_FixedRPM;
            commands[++ii] = *(fan_data)&0xFF;
            commands[++ii] = ( *( fan_data ) >> 8 ) & 0xFF;
        }
        else if ( new_fan_mode == PWM )
        {
            commands[++ii] = COOLIT_FixedPWM;
            commands[++ii] = CommandId++;
            commands[++ii] = WriteOneByte;
            commands[++ii] = FAN_FixedPWM;
            commands[++ii] = *(fan_data)&0xFF;
        }
        else if ( new_fan_mode == CUSTOM )
            commands[++ii] = COOLIT_Custom;
        else
            commands[++ii] = COOLIT_Default;
    }

    commands[0] = ii;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    if ( new_fan_mode == UNDEFINED )
    {
        *( fan_mode ) = response[4];
        ii = 0;
        memset( response, 0, sizeof( response ) );
        memset( commands, 0, sizeof( commands ) );

        if ( ( *(fan_mode)&0x0E ) == COOLIT_FixedRPM )
        {
            commands[++ii] = CommandId++;
            commands[++ii] = ReadTwoBytes;
            commands[++ii] = FAN_FixedRPM;
            commands[0] = ii;
            rr =
                dev->driver->write( handle, dev->write_endpoint, commands, 64 );
            rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );
            *( fan_data ) = ( response[3] << 8 ) + response[2];
        }
        else if ( ( *(fan_mode)&0x0E ) == COOLIT_FixedPWM )
        {
            commands[++ii] = CommandId++;
            commands[++ii] = ReadOneByte;
            commands[++ii] = FAN_FixedPWM;
            commands[0] = ii;
            rr =
                dev->driver->write( handle, dev->write_endpoint, commands, 64 );
            rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );
            *( fan_data ) = response[2];
        }
        else
            *( fan_data ) = 0;
    }

    return rr;
}

int corsairlink_coolit_fan_curve( struct corsair_device_info* dev,
                                  struct libusb_device_handle* handle,
                                  uint8_t selector, struct fan_table* fan )
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

    commands[++ii] = fan->t1;
    commands[++ii] = 0x00;
    commands[++ii] = fan->t2;
    commands[++ii] = 0x00;
    commands[++ii] = fan->t3;
    commands[++ii] = 0x00;
    commands[++ii] = fan->t4;
    commands[++ii] = 0x00;
    commands[++ii] = fan->t5;
    commands[++ii] = 0x00;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteThreeBytes;
    commands[++ii] = FAN_RPMTable;
    commands[++ii] = 0x0A;

    commands[++ii] = fan->s1;
    commands[++ii] = 0x00;
    commands[++ii] = fan->s2;
    commands[++ii] = 0x00;
    commands[++ii] = fan->s3;
    commands[++ii] = 0x00;
    commands[++ii] = fan->s4;
    commands[++ii] = 0x00;
    commands[++ii] = fan->s5;
    commands[++ii] = 0x00;

    commands[0] = ii;
    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    return rr;
}

int corsairlink_coolit_fan_speed( struct corsair_device_info* dev,
                                  struct libusb_device_handle* handle,
                                  uint8_t selector, uint16_t* speed,
                                  uint16_t* maxspeed )
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
    commands[++ii] = selector;

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
    *( speed ) = ( response[5] << 8 ) + response[4];
    *( maxspeed ) = ( response[9] << 8 ) + response[8];

    return rr;
}
