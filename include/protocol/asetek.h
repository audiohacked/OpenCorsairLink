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

#ifndef _PROTOCOL_ASETEK_H
#define _PROTOCOL_ASETEK_H

#include <stdint.h>

enum Asetek_Commands
{
    ChangeLed = 0x10,
    FanCurve = 0x11,
    UnknownCmd = 0x12,
    PumpMode = 0x13,
};

enum Asetek_FanCurve
{
    UnknownFanCurve = 0x00,
};

enum Asetek_PumpModes
{
    Asetek_Quiet = 0x28,
    Asetek_Performance = 0x42,
};

#define ASETEK_FAN_TABLE_QUIET( x ) \
    x[0].temperature = 0x14;        \
    x[1].temperature = 0x20;        \
    x[2].temperature = 0x28;        \
    x[3].temperature = 0x32;        \
    x[4].temperature = 0x37;        \
    x[5].temperature = 0x3c;        \
    x[0].speed = 0x19;              \
    x[1].speed = 0x27;              \
    x[2].speed = 0x32;              \
    x[3].speed = 0x4b;              \
    x[4].speed = 0x5a;              \
    x[5].speed = 0x64;

#define ASETEK_FAN_TABLE_BALANCED( x ) \
    x[0].temperature = 0x14;           \
    x[1].temperature = 0x20;           \
    x[2].temperature = 0x28;           \
    x[3].temperature = 0x32;           \
    x[4].temperature = 0x37;           \
    x[5].temperature = 0x3c;           \
    x[0].speed = 0x19;                 \
    x[1].speed = 0x27;                 \
    x[2].speed = 0x32;                 \
    x[3].speed = 0x4b;                 \
    x[4].speed = 0x5a;                 \
    x[5].speed = 0x64;

#define ASETEK_FAN_TABLE_EXTREME( x ) \
    x[0].temperature = 0x14;          \
    x[1].temperature = 0x19;          \
    x[2].temperature = 0x1e;          \
    x[3].temperature = 0x24;          \
    x[4].temperature = 0x30;          \
    x[5].temperature = 0x3c;          \
    x[0].speed = 0x32;                \
    x[1].speed = 0x32;                \
    x[2].speed = 0x4b;                \
    x[3].speed = 0x55;                \
    x[4].speed = 0x5d;                \
    x[5].speed = 0x64;

int
corsairlink_asetek_device_id(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint8_t* device_id );

int
corsairlink_asetek_name(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_size );

int
corsairlink_asetek_vendor(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_size );

int
corsairlink_asetek_product(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_size );

int
corsairlink_asetek_firmware_id(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* firmware,
    uint8_t firmware_size );

/* LED */
int
corsairlink_asetek_led_static_color(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct led_control* ctrl );

int
corsairlink_asetek_led_temperature(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct led_control* ctrl );

/* Pump */
int
corsairlink_asetek_pump_mode(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

int
corsairlink_asetek_pump_speed(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

/* Fan */
int
corsairlink_asetek_fan_count(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_asetek_fan_mode_read(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

// int corsairlink_asetek_fan_mode(
//     struct corsair_device_info* dev,
//     struct libusb_device_handle* handle,
//     struct fan_control* ctrl );

int
corsairlink_asetek_fan_mode_performance(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_asetek_fan_mode_balanced(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_asetek_fan_mode_quiet(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_asetek_fan_curve(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_asetek_fan_speed(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_asetek_fan_print_mode(
    uint8_t mode, uint16_t data, char* modestr, uint8_t modestr_size );

/* Temperature */
int
corsairlink_asetek_temperature(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t selector,
    double* temperature );

int
corsairlink_asetek_tempsensorscount(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t* temperature_sensors_count );

#endif
