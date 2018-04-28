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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libusb.h>
#include "lowlevel/asetek.h"
#include "device.h"
#include "driver.h"


int corsairlink_asetek_led_static_color(struct corsair_device_info *dev,
            struct libusb_device_handle *handle,
            struct color *color_led)
{
    int rr;
    uint8_t response[32];
    uint8_t commands[32] ;
    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    commands[0] = 0x10;
    commands[1] = color_led->red;
    commands[2] = color_led->green;
    commands[3] = color_led->blue;
    commands[4] = 0x00;
    commands[5] = 0xff;
    commands[6] = 0xff;
    commands[7] = 0xff;
    commands[8] = 0x00;
    commands[9] = 0x00;
    commands[10] = 0x37; // 0x37 = 55C, 0x2d = 45C
    commands[11] = 0x0a; //FadeThobInterval
    commands[12] = 0x05; //ThrobSequence
    commands[13] = 0x01; //LED Enable
    commands[14] = 0x00; //Fade Enable
    commands[15] = 0x00; //Throb Enable
    commands[16] = 0x00; //Warning_Enable
    commands[17] = 0x00; //BootMode
    commands[18] = 0x01; //FanType

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 19);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

    return rr;
}


int corsairlink_asetek_led_temperature(struct corsair_device_info *dev,
            struct libusb_device_handle *handle,
            struct led_temperatures *led_temps, struct color *led_color,
            struct color *color_fade, struct color *color_warning)
{
    int rr;
    uint8_t response[32];
    uint8_t commands[32] ;
    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    commands[0] = 0x10;
    commands[1] = led_color->red;
    commands[2] = led_color->green;
    commands[3] = led_color->blue;
    commands[4] = color_fade->blue;
    commands[5] = color_fade->green;
    commands[6] = color_fade->blue;
    commands[7] = color_warning->red;
    commands[8] = color_warning->green;
    commands[9] = color_warning->blue;
    commands[10] = led_temps->temp1; // 0x37 = ??, 0x2d = ??
    commands[11] = 10; //FadeThobInterval
    commands[12] = 5; //ThrobSequence
    commands[13] = 0x01; //LED Enable
    commands[14] = 0x00; //Fade Enable
    commands[15] = 0x00; //Throb Enable
    commands[16] = 0x01; //Warning_Enable
    commands[17] = 0x00; //BootMode
    commands[18] = 0x01; //FanType

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 19);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

    return rr;
}
