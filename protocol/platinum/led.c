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
#include "logic/options.h"
#include "lowlevel/platinum.h"
#include "protocol/platinum.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_platinum_change_led(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct led_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;

    commands[++ii] = CommandId++; // Command ID
    commands[++ii] = WriteOneByte; // Command Opcode
    commands[++ii] = LED_SelectCurrent; // Command data...
    commands[++ii] = 0;

    commands[++ii] = CommandId++; // Command ID
    commands[++ii] = WriteOneByte; // Command Opcode
    commands[++ii] = LED_Mode; // Command data...
    commands[++ii] = 0x00;

    commands[++ii] = CommandId++; // Command ID
    commands[++ii] = WriteThreeBytes; // Command Opcode
    commands[++ii] = LED_CycleColors; // Command data...
    commands[++ii] = 0x0C;

    commands[++ii] = ctrl->led_colors[0].red;
    commands[++ii] = ctrl->led_colors[0].green;
    commands[++ii] = ctrl->led_colors[0].blue;

    commands[++ii] = ctrl->led_colors[0].red;
    commands[++ii] = ctrl->led_colors[0].green;
    commands[++ii] = ctrl->led_colors[0].blue;

    commands[++ii] = ctrl->led_colors[0].red;
    commands[++ii] = ctrl->led_colors[0].green;
    commands[++ii] = ctrl->led_colors[0].blue;

    commands[++ii] = ctrl->led_colors[0].red;
    commands[++ii] = ctrl->led_colors[0].green;
    commands[++ii] = ctrl->led_colors[0].blue;

    commands[0] = ii; // Length

    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 64 );

    // fan_rpm = (long int) response[0]*16*16 + response[1];
    // pump_rpm = (response[8]*16*16)+response[9];
    // liquid_temp = (double) response[10] + (double) response[14]/10;

    return rr;
}
