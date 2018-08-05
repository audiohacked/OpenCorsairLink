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
#include "lowlevel/commanderpro.h"
#include "print.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_commanderpro_tempsensorscount(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t* temperature_sensors_count )
{
    int rr = 0;

    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x10;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 16 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    msg_debug2( "%02X %02X %02X %02X\n", response[1], response[2], response[3], response[4] );

    *( temperature_sensors_count ) = 4;
    // for (int ii = 1; ii <= 4; ++ii)
    // {
    //     if (response[ii] == 0x01)
    //     {
    //         *(temperature_sensors_count)++;
    //     }
    // }

    return rr;
}

int
corsairlink_commanderpro_temperature(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t sensor_index,
    double* temperature )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x11;
    commands[1] = sensor_index;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 16 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    uint16_t data;
    memcpy( &data, response + 1, 2 );
    *( temperature ) = (double)data / 100;
    // snprintf(temperature, temperature_str_len, "%5.2f C", (double)data/100);

    return rr;
}
