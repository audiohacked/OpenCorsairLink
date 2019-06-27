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
corsairlink_coolit_change_led(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct led_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = WriteOneByte; // Command Opcode
    commands[1] = LED_SelectCurrent; // Command data...
    commands[2] = 0;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );

    commands[0] = WriteOneByte; // Command Opcode
    commands[1] = LED_Mode; // Command data...
    commands[2] = 0x00;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 3 );

    commands[0x0] = WriteThreeBytes; // Command Opcode
    commands[0x1] = LED_CycleColors; // Command data...
    commands[0x2] = 0x0C;

    commands[0x3] = ctrl->led_colors[0].red;
    commands[0x4] = ctrl->led_colors[0].green;
    commands[0x5] = ctrl->led_colors[0].blue;

    commands[0x6] = ctrl->led_colors[0].red;
    commands[0x7] = ctrl->led_colors[0].green;
    commands[0x8] = ctrl->led_colors[0].blue;

    commands[0x9] = ctrl->led_colors[0].red;
    commands[0xA] = ctrl->led_colors[0].green;
    commands[0xB] = ctrl->led_colors[0].blue;

    commands[0xC] = ctrl->led_colors[0].red;
    commands[0xD] = ctrl->led_colors[0].green;
    commands[0xE] = ctrl->led_colors[0].blue;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 15 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    // fan_rpm = (long int) response[0]*16*16 + response[1];
    // pump_rpm = (response[8]*16*16)+response[9];
    // liquid_temp = (double) response[10] + (double) response[14]/10;

    return rr;
}
