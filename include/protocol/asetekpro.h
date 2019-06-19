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

#ifndef _PROTOCOL_ASETEKPRO_H
#define _PROTOCOL_ASETEKPRO_H

#include <stdint.h>

/*
 * These two defines are used to translage options into different item:
 *   - an enum for values
 *   - a list of string values 
 *
 */
#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

// This will be used to build AsetekProSpeed and AsetekProSpeed_String
#define FOREACH_ASETEKPROSPEED(speed) \
    speed(AsetekProSpeedSlow) \
    speed(AsetekProSpeedMedium) \
    speed(AsetekProSpeedFast)

enum AsetekProSpeed
{
    FOREACH_ASETEKPROSPEED(GENERATE_ENUM)
};

static const char *AsetekProSpeed_String[] =
{
    FOREACH_ASETEKPROSPEED(GENERATE_STRING)
};

// This will be used to build AsetekProPumpNodes and AsetekProPumpModes_String
#define FOREACH_ASETEKPROPUMPMODES(mode) \
	mode(AsetekProPumpQuiet) \
	mode(AsetekProPumpBalanced) \
	mode(AsetekProPumpPerformance)

enum AsetekProPumpModes
{
    FOREACH_ASETEKPROPUMPMODES(GENERATE_ENUM)
};

static const char *AsetekProPumpModes_String[] =
{
    FOREACH_ASETEKPROPUMPMODES(GENERATE_STRING)
};

enum AsetekProOperations
{
    AsetekProPumpSpeedWrite = 0x30,
    AsetekProPumpSpeedRead = 0x31,
    AsetekProPumpModeWrite = 0x32,
    AsetekProPumpModeRead = 0x33,
    AsetekProFanWrite = 0x40,
    AsetekProFanRead = 0x41,
    AsetekProFanFixedPWMWrite = 0x42,
    AsetekProFanFixedRPMWrite = 0x43,
    AsetekProReadTemp = 0xa9,
    AsetekProReadFirmwareVersion = 0xaa,
    AsetekProReadHardwareVersion = 0xab
};


#define ASETEKPRO_FAN_TABLE_QUIET( x ) \
    x[0].temperature = 0x10;        \
    x[1].temperature = 0x14;        \
    x[2].temperature = 0x20;        \
    x[3].temperature = 0x28;        \
    x[4].temperature = 0x32;        \
    x[5].temperature = 0x37;        \
    x[6].temperature = 0x3c;        \
    x[0].speed = 0x19;              \
    x[1].speed = 0x19;              \
    x[2].speed = 0x27;              \
    x[3].speed = 0x32;              \
    x[4].speed = 0x4b;              \
    x[5].speed = 0x5a;              \
    x[6].speed = 0x64;

#define ASETEKPRO_FAN_TABLE_BALANCED( x ) \
    x[0].temperature = 0x10;           \
    x[1].temperature = 0x14;           \
    x[2].temperature = 0x20;           \
    x[3].temperature = 0x28;           \
    x[4].temperature = 0x32;           \
    x[5].temperature = 0x37;           \
    x[6].temperature = 0x3c;           \
    x[0].speed = 0x19;                 \
    x[1].speed = 0x19;                 \
    x[2].speed = 0x27;                 \
    x[3].speed = 0x32;                 \
    x[4].speed = 0x4b;                 \
    x[5].speed = 0x5a;                 \
    x[6].speed = 0x64;

#define ASETEKPRO_FAN_TABLE_EXTREME( x ) \
    x[0].temperature = 0x10;          \
    x[1].temperature = 0x14;          \
    x[2].temperature = 0x19;          \
    x[3].temperature = 0x1e;          \
    x[4].temperature = 0x24;          \
    x[5].temperature = 0x30;          \
    x[6].temperature = 0x3c;          \
    x[0].speed = 0x32;                \
    x[1].speed = 0x32;                \
    x[2].speed = 0x32;                \
    x[3].speed = 0x4b;                \
    x[4].speed = 0x55;                \
    x[5].speed = 0x5d;                \
    x[6].speed = 0x64;

int
corsairlink_asetekpro_firmware_id(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* firmware,
    uint8_t firmware_size );

int
corsairlink_asetekpro_hardware_version(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle);

/* Fan */
int
corsairlink_asetekpro_fan_count(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_asetekpro_fan_mode_read(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

// int corsairlink_asetekpro_fan_mode(
//     struct corsair_device_info* dev,
//     struct libusb_device_handle* handle,
//     struct fan_control* ctrl );

int
corsairlink_asetekpro_fan_mode_performance(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_asetekpro_fan_mode_balanced(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_asetekpro_fan_mode_quiet(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_asetekpro_fan_curve(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_asetekpro_fan_speed(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_asetekpro_fan_print_mode(
    uint8_t mode, uint16_t data, char* modestr, uint8_t modestr_size );


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
    
int
corsairlink_asetekpro_fan_mode_rpm(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_asetekpro_fan_mode_pwm(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

/* pump */
int
corsairlink_asetekpro_pump_mode_read(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

int
corsairlink_asetekpro_pump_mode_quiet(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

int
corsairlink_asetekpro_pump_mode_balanced(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

int
corsairlink_asetekpro_pump_mode_performance(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

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
