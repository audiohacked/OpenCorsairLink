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
#include "../../lowlevel/hid.h"
#include "../../device.h"
#include "../../driver.h"
#include "../../print.h"
#include "core.h"

int corsairlink_hid_pump_mode(struct corsair_device_info *dev, struct libusb_device_handle *handle,
            uint8_t *pump_mode)
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];

    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    uint8_t new_pump_mode = *(pump_mode);

    uint8_t ii = 0;

    if (new_pump_mode == 0)
    {
        commands[++ii] = CommandId++;
        commands[++ii] = WriteOneByte;
        commands[++ii] = FAN_Select;
        commands[++ii] = dev->pump_index;

        commands[++ii] = CommandId++;
        commands[++ii] = ReadOneByte;
        commands[++ii] = FAN_Mode;

        commands[0] = ii;

        rr = dev->driver->write(handle, dev->write_endpoint, commands, 64);
        rr = dev->driver->read(handle, dev->read_endpoint, response, 64);

        *(pump_mode) = response[4];
    } else
    {
        commands[++ii] = CommandId++;
        commands[++ii] = WriteOneByte;
        commands[++ii] = FAN_Select;
        commands[++ii] = dev->pump_index;

        commands[++ii] = CommandId++;
        commands[++ii] = WriteOneByte;
        commands[++ii] = FAN_Mode;

        if (dev->device_id != 0x42)
        {
            if (new_pump_mode == PERFORMANCE)
                commands[++ii] = HID_Performance;
            else if (new_pump_mode == BALANCED)
                commands[++ii] = HID_Balanced;
            else if (new_pump_mode == QUIET)
                commands[++ii] = HID_Quiet;
            else if (new_pump_mode == DEFAULT)
                commands[++ii] = HID_Default;
            else commands[++ii] = HID_Default;
        } else
        {
            // My H110i Extreme with device_id 0x42 requires setting fixed RPM
            commands[++ii] = HID_FixedRPM;
            commands[++ii] = CommandId++;
            commands[++ii] = WriteTwoBytes;
            commands[++ii] = FAN_FixedRPM;

            if (new_pump_mode == PERFORMANCE)
            {
                // CorsairLink software sets to 2950 => 0x0B86
                commands[++ii] = 0x86;
                commands[++ii] = 0x0B;
            }
            else if (new_pump_mode == QUIET)
            {
                // CorsairLink software sets to 2350 => 0x092E
                commands[++ii] = 0x2E;
                commands[++ii] = 0x09;
            }
            else
            {
                // For any other mode, set to 2650 => 0x0A5A
                commands[++ii] = 0x5A;
                commands[++ii] = 0x0A;
            }
        }
        commands[0] = ii;

        rr = dev->driver->write(handle, dev->write_endpoint, commands, 64);
        rr = dev->driver->read(handle, dev->read_endpoint, response, 64);
    }

    return rr;
}

int corsairlink_hid_pump_speed(struct corsair_device_info *dev, struct libusb_device_handle *handle,
            uint16_t *speed, uint16_t *maxspeed)
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    uint8_t ii = 0;

    commands[++ii] = CommandId++;
    commands[++ii] = WriteOneByte;
    commands[++ii] = FAN_Select;
    commands[++ii] = dev->pump_index;

    commands[++ii] = CommandId++;
    commands[++ii] = ReadTwoBytes;
    commands[++ii] = FAN_ReadRPM;

    commands[++ii] = CommandId++;
    commands[++ii] = ReadTwoBytes;
    commands[++ii] = FAN_MaxRecordedRPM;

    commands[0] = ii;
    rr = dev->driver->write(handle, dev->write_endpoint, commands, 64);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 64);

    msg_debug("Speed: %02X %02X\n", response[5], response[4]);
    msg_debug("Max Speed: %02X %02X\n", response[9], response[8]);
    *(speed) = (response[5]<<8) + response[4];
    *(maxspeed) = (response[9]<<8) + response[8];

    return rr;
}
