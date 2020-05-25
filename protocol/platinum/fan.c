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
#include "logic/print.h"
#include "logic/options.h"
#include "lowlevel/platinum.h"
#include "protocol/platinum.h"
#include "protocol/crc.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_platinum_fan_count(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr = 0;
    // uint8_t response[64];
    // uint8_t commands[64];
    // memset( response, 0, sizeof( response ) );
    // memset( commands, 0, sizeof( commands ) );
    //
    // uint8_t ii = 0;
    //
    // commands[++ii] = CommandId++;
    // commands[++ii] = ReadOneByte;
    // commands[++ii] = FAN_Count;
    //
    // commands[0] = ii;
    //
    // rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    // rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 64 );

    ctrl->fan_count = 2; // we subtract 1 because count includes pump

    return rr;
}

int
corsairlink_platinum_fan_print_mode(
    uint8_t mode, uint16_t data, char* modestr, uint8_t modestr_size )
{
    int rr = 0;
    // uint8_t isConnected = mode & 0x80;
    // uint8_t is4pin = mode & 0x01;
    // uint8_t real_mode = mode & 0x0E;
    //
    // if ( !isConnected )
    //     snprintf( modestr, modestr_size, "Not connected or failed" );
    // else if ( real_mode == COOLIT_Performance )
    //     snprintf( modestr, modestr_size, "Performance Mode (%s)", is4pin ? "4PIN" : "3PIN" );
    // else if ( real_mode == COOLIT_Balanced )
    //     snprintf( modestr, modestr_size, "Balanced Mode (%s)", is4pin ? "4PIN" : "3PIN" );
    // else if ( real_mode == COOLIT_Quiet )
    //     snprintf( modestr, modestr_size, "Quiet Mode (%s)", is4pin ? "4PIN" : "3PIN" );
    // else if ( real_mode == COOLIT_Default )
    //     snprintf( modestr, modestr_size, "Default Mode (%s)", is4pin ? "4PIN" : "3PIN" );
    // else if ( real_mode == COOLIT_FixedPWM )
    //     snprintf(
    //         modestr, modestr_size, "Fixed PWM Mode (%s) set to %d%%", is4pin ? "4PIN" : "3PIN",
    //         ( data + 1 ) * 100 / 256 );
    // else if ( real_mode == COOLIT_FixedRPM )
    //     snprintf(
    //         modestr, modestr_size, "Fixed RPM Mode (%s) set to %d", is4pin ? "4PIN" : "3PIN",
    //         data );
    // else if ( real_mode == COOLIT_Custom )
    //     snprintf( modestr, modestr_size, "Custom Curve Mode (%s)", is4pin ? "4PIN" : "3PIN" );
    
    snprintf( modestr, modestr_size, "Unknown (STUB)");

    return rr;
}

int
corsairlink_platinum_fan_mode_read(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    // uint8_t response[64];
    // uint8_t commands[64];
    // memset( response, 0, sizeof( response ) );
    // memset( commands, 0, sizeof( commands ) );
    //
    // uint8_t ii = 0;
    // commands[++ii] = CommandId++;
    // commands[++ii] = WriteOneByte;
    // commands[++ii] = FAN_Select;
    // commands[++ii] = ctrl->channel;
    //
    // commands[++ii] = CommandId++;
    // commands[++ii] = ReadOneByte;
    // commands[++ii] = FAN_Mode;
    //
    // commands[0] = ii;
    //
    // rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    // rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 64 );

    ctrl->mode = 0x00;

    return rr;
}

int
corsairlink_platinum_fan_mode_read_rpm(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0x00] = 0x3F;
    commands[0x01] = 0x78;

    commands[0x3F] = crc8ccitt(commands+1, 62);

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 64 );

    ctrl->max_speed = 0xFFFF; // 

    switch (ctrl->fan_count) {
        case 1:
            msg_debug2( "Speed: %02X %02X\n", response[0x0F], response[0x10] );
            // msg_debug2( "Max Speed: %02X %02X\n", response[9], response[8] );
            ctrl->speed_rpm = ( response[0x10] << 8 ) + response[0x0F];
            // ctrl->max_speed = ( response[9] << 8 ) + response[8];
            break;
        case 2:
            msg_debug2( "Speed: %02X %02X\n", response[0x16], response[0x17] );
            // msg_debug2( "Max Speed: %02X %02X\n", response[9], response[8] );
            ctrl->speed_rpm = ( response[0x17] << 8 ) + response[0x16];
            // ctrl->max_speed = ( response[9] << 8 ) + response[8];
            break;
        default:
            msg_debug2("Fan Index out of bounds");
            break;        
    }

    return rr;
}

int
corsairlink_platinum_fan_speed(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    int rr;

    rr = corsairlink_platinum_fan_mode_read_rpm( dev, handle, ctrl );

    return rr;
}
