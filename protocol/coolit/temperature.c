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
#include "protocol/coolit.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_coolit_tempsensorscount(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t* temperature_sensors_count )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;

    commands[++ii] = CommandId++; // Command ID
    commands[++ii] = ReadOneByte; // Command Opcode
    commands[++ii] = TEMP_CountSensors; // Command data...

    commands[0] = ii; // Length

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    *( temperature_sensors_count ) = response[2];

    return rr;
}

int
corsairlink_coolit_temperature(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t selector,
    double* temperature )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;

    commands[++ii] = CommandId++; // Command ID
    commands[++ii] = WriteOneByte; // Command Opcode
    commands[++ii] = TEMP_SelectActiveSensor; // Command data...
    commands[++ii] = selector;

    commands[++ii] = CommandId++; // Command ID
    commands[++ii] = ReadTwoBytes; // Command Opcode
    commands[++ii] = TEMP_Read; // Command data...

    commands[0] = ii; // Length

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    // *(temperature) = (response[5]<<8) + response[4];
    *( temperature ) = (double)response[5] + ( (double)response[4] / 256 );
    // snprintf(temperature, temperature_str_len, "%d.%d C", response[5],
    // response[4]);

    return rr;
}
