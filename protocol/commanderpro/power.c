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

/*! \file protocol/commanderpro/power.c
 *  \brief Power Routines for Commander Pro
 */
#include "device.h"
#include "driver.h"
#include "lowlevel/commanderpro.h"
#include "print.h"
#include "protocol/commanderpro.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_commanderpro_voltage(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t sensor_index,
    double* voltage )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x12;
    commands[1] = sensor_index;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    msg_debug2( "%02X %02X %02X\n", response[0], response[1], response[2] );

    uint16_t data = ( response[1] << 8 ) + response[2];
    *( voltage ) = (double)data / 1000;

    // snprintf(voltage, voltage_str_len, "%2.3f V", volts);

    return 0;
}
