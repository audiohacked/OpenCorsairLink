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

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int corsairlink_commanderpro_get_led_setup_mask(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint8_t led_channel )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x30;
    commands[1] = led_channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    return rr;
}

int corsairlink_commanderpro_set_led_rgb_colour(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t led_channel,
    struct color* color_led )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x31;
    commands[1] = led_channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    return rr;
}

int corsairlink_commanderpro_set_led_rgb_values(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t led_channel,
    struct color* color_led )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x32;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    return rr;
}

int corsairlink_commanderpro_set_commit(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t led_channel,
    struct color* color_led )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x33;
    commands[1] = 0xFF;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    return rr;
}

int corsairlink_commanderpro_set_begin(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t led_channel,
    struct color* color_led )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x34;
    commands[1] = led_channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    return rr;
}

int corsairlink_commanderpro_set_led_config(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t led_channel,
    uint8_t strip_count,
    uint8_t led_type,
    uint8_t mode,
    uint8_t speed,
    uint8_t direction,
    struct color led_color,
    struct led_temperatures led_temperature )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0x00] = 0x35;
    commands[0x01] = led_channel; // CLNP led_channel
    commands[0x02] = strip_count * 10;
    commands[0x03] = led_type; // 0x0A = LED Strip, 0x0C = HD Fan, 0x01 = SP
                               // Fan, 0x04 = ML Fan
    commands[0x04] = mode;
    commands[0x05] = speed;
    commands[0x06] = direction;
    commands[0x07] = color_change_style;
    commands[0x08] = 0xFF;

    commands[0x09] = led_color[0]->red;
    commands[0x0A] = led_color[0]->green;
    commands[0x0B] = led_color[0]->blue;

    commands[0x0C] = led_color[1]->red;
    commands[0x0D] = led_color[1]->green;
    commands[0x0E] = led_color[1]->blue;

    commands[0x0F] = led_color[2]->red;
    commands[0x10] = led_color[2]->green;
    commands[0x11] = led_color[2]->blue;

    commands[0x12] = ( led_temperature->temp1 >> 8 );
    commands[0x13] = ( led_temperature->temp1 && 0xFF );

    commands[0x14] = ( led_temperature->temp2 >> 8 );
    commands[0x15] = ( led_temperature->temp2 && 0xFF );

    commands[0x16] = ( led_temperature->temp3 >> 8 );
    commands[0x17] = ( led_temperature->temp3 && 0xFF );

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    return rr;
}

int corsairlink_commanderpro_set_led_temperature(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t led_channel,
    struct color* color_led )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x36;
    commands[1] = led_channel;
    commands[2] = led_channel;
    commands[3] = 0x0A;
    commands[4] = 0x28;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    return rr;
}

int corsairlink_commanderpro_set_led_group_reset(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t led_channel,
    struct color* color_led )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x37;
    commands[1] = led_channel;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    return rr;
}

int corsairlink_commanderpro_set_port_state(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t led_channel,
    struct color* color_led )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x38;
    commands[1] = led_channel;
    commands[2] = 0x01;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    return rr;
}

int corsairlink_commanderpro_set_port_brightness(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t led_channel,
    struct color* color_led )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x39;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    return rr;
}

int corsairlink_commanderpro_set_port_led_count(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t led_channel,
    struct color* color_led )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x3A;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    return rr;
}

int corsairlink_commanderpro_set_port_protocol(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t led_channel,
    struct color* color_led )
{
    int rr;
    uint8_t response[16];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0] = 0x3B;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 16 );

    return rr;
}
