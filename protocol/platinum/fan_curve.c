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

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int
corsairlink_platinum_fan_mode_performance(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    // PLATINUM_FAN_TABLE_EXTREME(*ctrl);
    return corsairlink_platinum_fan_curve(dev, handle, ctrl);
}

int
corsairlink_platinum_fan_mode_balanced(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    // PLATINUM_FAN_TABLE_BALANCED(*ctrl);
    return corsairlink_platinum_fan_curve(dev, handle, ctrl);
}

int
corsairlink_platinum_fan_mode_quiet(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    // PLATINUM_FAN_TABLE_QUIET(*ctrl);
    return corsairlink_platinum_fan_curve(dev, handle, ctrl);
}

int
corsairlink_platinum_fan_mode_custom(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    return corsairlink_platinum_fan_curve(dev, handle, ctrl);
}

int
corsairlink_platinum_fan_mode_default(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    return corsairlink_platinum_fan_mode_balanced(dev, handle, ctrl);
}

int
corsairlink_platinum_fan_curve(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
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

    commands[++ii] = ctrl->table[0].temperature;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[1].temperature;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[2].temperature;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[3].temperature;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[4].temperature;
    commands[++ii] = 0x00;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteThreeBytes;
    commands[++ii] = FAN_RPMTable;
    commands[++ii] = 0x0A;

    commands[++ii] = ctrl->table[0].speed;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[1].speed;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[2].speed;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[3].speed;
    commands[++ii] = 0x00;
    commands[++ii] = ctrl->table[4].speed;
    commands[++ii] = 0x00;

    commands[0] = ii;
    rr = dev->lowlevel->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->lowlevel->read( handle, dev->read_endpoint, response, 64 );

    return rr;
}
