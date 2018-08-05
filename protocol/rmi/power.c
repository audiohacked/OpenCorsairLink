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

/*! \file protocol/rmi/power.c
 *  \brief Power Routines for RMi Series of Power Supplies
 */
#include "device.h"
#include "driver.h"
#include "lowlevel/rmi.h"
#include "print.h"
#include "protocol/rmi.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_rmi_sensor_select(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint8_t sensor_select )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 1;

    commands[0] = 0x02; // Length
    commands[1] = 0x00; // Command Opcode: Output X Select
    commands[2] = sensor_select; // Command data...

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    return 0;
}

int
corsairlink_rmi_output_volts(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t sensor_select,
    double* volts )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x03; // Length
    commands[1] = 0x8B; // Command Opcode: Output X Volts
    commands[2] = 0x00; // Command data...
    commands[3] = 0x00;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    msg_debug2(
        "%02X %02X %02X %02X %02X %02X\n", response[0], response[1], response[2], response[3],
        response[4], response[5] );

    uint16_t data;
    memcpy( &data, response + 2, 2 );
    *( volts ) = convert_bytes_double( data );
    // snprintf(volts, volts_str_len, "%5.2f V", convert_bytes_double(data));

    return 0;
}

int
corsairlink_rmi_output_amps(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t sensor_select,
    double* amps )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t i = 1;

    commands[0] = 0x03; // Length
    commands[1] = 0x8C; // Command Opcode: Output X Amps
    commands[2] = 0x00; // Command data...
    commands[3] = 0x00;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    msg_debug2(
        "%02X %02X %02X %02X %02X %02X\n", response[0], response[1], response[2], response[3],
        response[4], response[5] );

    uint16_t data;
    memcpy( &data, response + 2, 2 );
    *( amps ) = convert_bytes_double( data );
    // snprintf(amps, amps_str_len, "%5.2f A", convert_bytes_double(data));

    return 0;
}

int
corsairlink_rmi_output_watts(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t sensor_select,
    double* watts )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 1;

    commands[0] = 0x03; // Length
    commands[1] = 0x96; // Command Opcode: Output X Watts
    commands[2] = 0x00; // Command data...
    commands[3] = 0x00;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    msg_debug2(
        "%02X %02X %02X %02X %02X %02X\n", response[0], response[1], response[2], response[3],
        response[4], response[5] );

    uint16_t data;
    memcpy( &data, response + 2, 2 );
    *( watts ) = convert_bytes_double( data );
    // snprintf(watts, watts_str_len, "%5.2f W", convert_bytes_double(data));

    return 0;
}

int
corsairlink_rmi_power_supply_voltage(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, double* volts )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 1;

    commands[0] = 0x03; // Length
    commands[1] = 0x88; // Command Opcode: Supply Volts
    commands[2] = 0x00; // Command data...
    commands[3] = 0x00;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 4 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    msg_debug2(
        "%02X %02X %02X %02X %02X %02X\n", response[0], response[1], response[2], response[3],
        response[4], response[5] );

    uint16_t data = 0;
    memcpy( &data, response + 2, 2 );
    *( volts ) = convert_bytes_double( data );
    // snprintf(volts, volts_str_len, "%5.2f V", convert_bytes_double(data));

    return 0;
}

int
corsairlink_rmi_power_total_wattage(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, double* watts )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 1;

    commands[0] = 0x03; // Length
    commands[1] = 0xEE; // Command Opcode: Total Watts
    commands[2] = 0x00; // Command data...
    commands[3] = 0x00;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    msg_debug2(
        "%02X %02X %02X %02X %02X %02X\n", response[0], response[1], response[2], response[3],
        response[4], response[5] );

    uint16_t data;
    memcpy( &data, response + 2, 2 );
    *( watts ) = convert_bytes_double( data );
    // snprintf(watts, watts_str_len, "%5.2f W", convert_bytes_double(data));

    return 0;
}
