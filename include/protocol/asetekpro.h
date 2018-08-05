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

#ifndef _PROTOCOL_ASETEKPRO_H
#define _PROTOCOL_ASETEKPRO_H

#include <stdint.h>

enum AsetekProSpeed
{
    AsetekProSpeedSlow = 0,
    AsetekProSpeedMedium,
    AsetekProSpeedFast,
};

int
corsairlink_asetekpro_firmware_id(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* firmware,
    uint8_t firmware_size );

/* fan */
int
corsairlink_asetekpro_fan_speed(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

/* led */
int
corsairlink_asetekpro_led_static_color(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct led_control* ctrl );

int
corsairlink_asetekpro_led_blink(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct led_control* ctrl );

int
corsairlink_asetekpro_led_color_pulse(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct led_control* ctrl );

int
corsairlink_asetekpro_led_color_shift(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct led_control* ctrl );

int
corsairlink_asetekpro_led_rainbow(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct led_control* ctrl );

int
corsairlink_asetekpro_led_temperature(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct led_control* ctrl );

/* pump */
int
corsairlink_asetekpro_pump_speed(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

/* temperature */
int
corsairlink_asetekpro_tempsensorscount(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t* temperature_sensors_count );

int
corsairlink_asetekpro_temperature(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t selector,
    double* temperature );
#endif
