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
#include "print.h"
#include "protocol/asetekpro/core.h"

int corsairlink_asetekpro_led_static_color(struct corsair_device_info *dev,
            struct libusb_device_handle *handle,
            struct color *color_led)
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    commands[0] = 0x56;
    commands[1] = 0x02;

    commands[2] = color_led->red;
    commands[3] = color_led->green;
    commands[4] = color_led->blue;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 5);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 3);

    msg_debug("%02X %02X %02X\n", response[0], response[1], response[2]);

    if (response[0] != 0x56 || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    commands[0] = 0x55;
    commands[1] = 0x01;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 2);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 3);

    msg_debug("%02X %02X %02X\n", response[0], response[1], response[2]);

    if (response[0] != 0x55 || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    return rr;
}

int corsairlink_asetekpro_led_blink(struct corsair_device_info *dev,
            struct libusb_device_handle *handle, uint8_t speed, uint8_t led_count,
            struct color *led1, struct color *led2, struct color *led3,
            struct color *led4, struct color *led5, struct color *led6,
            struct color *led7)
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    commands[0] = 0x56;
    commands[1] = led_count;

    commands[2] = led1->red;
    commands[3] = led1->green;
    commands[4] = led1->blue;

    commands[5] = led2->red;
    commands[6] = led2->green;
    commands[7] = led2->blue;

    commands[8] = led3->red;
    commands[9] = led3->green;
    commands[10] = led3->blue;

    commands[11] = led4->red;
    commands[12] = led4->green;
    commands[13] = led4->blue;

    commands[14] = led5->red;
    commands[15] = led5->green;
    commands[16] = led5->blue;

    commands[17] = led6->red;
    commands[18] = led6->green;
    commands[19] = led6->blue;

    commands[20] = led7->red;
    commands[21] = led7->green;
    commands[22] = led7->blue;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 23);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 3);

    msg_debug("%02X %02X %02X\n", response[0], response[1], response[2]);

    if (response[0] != 0x56 || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    commands[0] = 0x53;
    switch(speed) {
        case AsetekProSpeedSlow:
            commands[1] = 0x0F;
            break;
        case AsetekProSpeedMedium:
            commands[1] = 0x0A;
            break;
        case AsetekProSpeedFast:
        default:
            commands[1] = 0x05;
    }

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 2);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 3);

    msg_debug("%02X %02X %02X\n", response[0], response[1], response[2]);

    if (response[0] != 0x53 || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    commands[0] = 0x58;
    commands[1] = 0x01;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 2);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 3);

    msg_debug("%02X %02X %02X\n", response[0], response[1], response[2]);

    if (response[0] != 0x58 || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    return rr;
}

int corsairlink_asetekpro_led_color_pulse(struct corsair_device_info *dev,
            struct libusb_device_handle *handle, uint8_t speed, uint8_t led_count,
            struct color *led1, struct color *led2, struct color *led3,
            struct color *led4, struct color *led5, struct color *led6,
            struct color *led7)
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    commands[0] = 0x56;
    commands[1] = led_count;

    commands[2] = led1->red;
    commands[3] = led1->green;
    commands[4] = led1->blue;

    commands[5] = led2->red;
    commands[6] = led2->green;
    commands[7] = led2->blue;

    commands[8] = led3->red;
    commands[9] = led3->green;
    commands[10] = led3->blue;

    commands[11] = led4->red;
    commands[12] = led4->green;
    commands[13] = led4->blue;

    commands[14] = led5->red;
    commands[15] = led5->green;
    commands[16] = led5->blue;

    commands[17] = led6->red;
    commands[18] = led6->green;
    commands[19] = led6->blue;

    commands[20] = led7->red;
    commands[21] = led7->green;
    commands[22] = led7->blue;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 23);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 3);

    msg_debug("%02X %02X %02X\n", response[0], response[1], response[2]);

    if (response[0] != 0x56 || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    commands[0] = 0x53;
    switch(speed) {
        case AsetekProSpeedSlow:
            commands[1] = 0x50;
            break;
        case AsetekProSpeedMedium:
            commands[1] = 0x37;
            break;
        case AsetekProSpeedFast:
        default:
            commands[1] = 0x1E;
    }

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 2);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 3);

    msg_debug("%02X %02X %02X\n", response[0], response[1], response[2]);

    if (response[0] != 0x53 || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    commands[0] = 0x52;
    commands[1] = 0x01;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 2);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 3);

    msg_debug("%02X %02X %02X\n", response[0], response[1], response[2]);

    if (response[0] != 0x52 || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    return rr;
}

int corsairlink_asetekpro_led_color_shift(struct corsair_device_info *dev,
            struct libusb_device_handle *handle, uint8_t speed, uint8_t led_count,
            struct color *led1, struct color *led2, struct color *led3,
            struct color *led4, struct color *led5, struct color *led6,
            struct color *led7)
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    commands[0] = 0x56;
    commands[1] = led_count;

    commands[2] = led1->red;
    commands[3] = led1->green;
    commands[4] = led1->blue;

    commands[5] = led2->red;
    commands[6] = led2->green;
    commands[7] = led2->blue;

    commands[8] = led3->red;
    commands[9] = led3->green;
    commands[10] = led3->blue;

    commands[11] = led4->red;
    commands[12] = led4->green;
    commands[13] = led4->blue;

    commands[14] = led5->red;
    commands[15] = led5->green;
    commands[16] = led5->blue;

    commands[17] = led6->red;
    commands[18] = led6->green;
    commands[19] = led6->blue;

    commands[20] = led7->red;
    commands[21] = led7->green;
    commands[22] = led7->blue;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 23);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 3);

    msg_debug("%02X %02X %02X\n", response[0], response[1], response[2]);

    if (response[0] != 0x56 || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    commands[0] = 0x53;
    switch(speed) {
        case AsetekProSpeedSlow:
            commands[1] = 0x46;
            break;
        case AsetekProSpeedMedium:
            commands[1] = 0x28;
            break;
        case AsetekProSpeedFast:
        default:
            commands[1] = 0x0F;
    }

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 2);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 3);

    msg_debug("%02X %02X %02X\n", response[0], response[1], response[2]);

    if (response[0] != 0x53 || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    commands[0] = 0x55;
    commands[1] = 0x01;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 2);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 3);

    msg_debug("%02X %02X %02X\n", response[0], response[1], response[2]);

    if (response[0] != 0x55 || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    return rr;
}

int corsairlink_asetekpro_led_rainbow(struct corsair_device_info *dev,
            struct libusb_device_handle *handle, uint8_t speed)
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    commands[0] = 0x53;
    switch(speed) {
        case AsetekProSpeedSlow:
            commands[1] = 0x30;
            break;
        case AsetekProSpeedMedium:
            commands[1] = 0x18;
            break;
        case AsetekProSpeedFast:
        default:
            commands[1] = 0x0C;
    }

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 2);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 3);

    msg_debug("%02X %02X %02X\n", response[0], response[1], response[2]);

    if (response[0] != 0x53 || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    commands[0] = 0x55;
    commands[1] = 0x01;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 2);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 3);

    msg_debug("%02X %02X %02X\n", response[0], response[1], response[2]);

    if (response[0] != 0x55 || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    return rr;
}

int corsairlink_asetekpro_led_temperature(struct corsair_device_info *dev,
            struct libusb_device_handle *handle,
            struct led_temperatures *led_temps, struct color *temp1_led,
            struct color *temp2_led, struct color *temp3_led)
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    commands[0] = 0x5f;
    commands[1] = led_temps->temp1;
    commands[2] = 0x00;
    commands[3] = led_temps->temp2;
    commands[4] = 0x00;
    commands[5] = led_temps->temp3;
    commands[6] = 0x00;
    commands[7] = temp1_led->red;
    commands[8] = temp1_led->green;
    commands[9] = temp1_led->blue;
    commands[10] = temp2_led->red;
    commands[11] = temp2_led->green;
    commands[12] = temp2_led->blue;
    commands[13] = temp3_led->red;
    commands[14] = temp3_led->green;
    commands[15] = temp3_led->blue;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 16);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 6);

    msg_debug("%02X %02X %02X %02X %02X %02X\n", response[0], response[1],
                response[2], response[3], response[4], response[5]);

    if (response[0] != 0x5f || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    commands[0] = 0x5E;
    commands[1] = 0x01;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 2);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 3);

    msg_debug("%02X %02X %02X\n", response[0], response[1], response[2]);

    if (response[0] != 0x5E || response[1] != 0x12 || response[2] != 0x34)
    {
        msg_debug("Bad Response\n");
    }

    return rr;
}
