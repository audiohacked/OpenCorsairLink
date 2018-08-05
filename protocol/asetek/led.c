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
#include "lowlevel/asetek.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_asetek_led_static_color(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct led_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x10;
    commands[1] = ctrl->led_colors[0].red;
    commands[2] = ctrl->led_colors[0].green;
    commands[3] = ctrl->led_colors[0].blue;
    commands[4] = 0x00;
    commands[5] = 0xff;
    commands[6] = 0xff;
    commands[7] = 0xff;
    commands[8] = 0x00;
    commands[9] = 0x00;
    commands[10] = 0x37; // 0x37 = 55C, 0x2d = 45C
    commands[11] = 0x0a; // FadeThobInterval
    commands[12] = 0x05; // ThrobSequence
    commands[13] = 0x01; // LED Enable
    commands[14] = 0x00; // Fade Enable
    commands[15] = 0x00; // Throb Enable
    commands[16] = 0x00; // Warning_Enable
    commands[17] = 0x00; // BootMode
    commands[18] = 0x01; // FanType

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 19 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 32 );

    return rr;
}

int
corsairlink_asetek_led_temperature(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct led_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x10;
    commands[1] = ctrl->led_colors[0].red;
    commands[2] = ctrl->led_colors[0].green;
    commands[3] = ctrl->led_colors[0].blue;
    commands[4] = ctrl->led_colors[1].blue; // fade
    commands[5] = ctrl->led_colors[1].green;
    commands[6] = ctrl->led_colors[1].blue;
    commands[7] = ctrl->led_colors[2].red; // warning
    commands[8] = ctrl->led_colors[2].green;
    commands[9] = ctrl->led_colors[2].blue;
    commands[10] = ctrl->temperatures[2]; // 0x37 = ??, 0x2d = ??
    commands[11] = 10; // FadeThobInterval
    commands[12] = 5; // ThrobSequence
    commands[13] = 0x01; // LED Enable
    commands[14] = 0x00; // Fade Enable
    commands[15] = 0x00; // Throb Enable
    commands[16] = 0x01; // Warning_Enable
    commands[17] = 0x00; // BootMode
    commands[18] = 0x01; // FanType

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 19 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 32 );

    return rr;
}
