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

#ifndef _PROTOCOL_PLATINUM_H
#define _PROTOCOL_PLATINUM_H

#define PlatinumCoolingNode 0x38
#define PlatinumH100 0x3a
#define PlatinumH100i 0x3c
#define PlatinumH100iGT 0x40
#define PlatinumH110i 0x42
#define PlatinumH110iGT 0x41
#define PlatinumH80 0x37
#define PlatinumH80i 0x3b
#define PlatinumLightingNode 0x39
#define PlatinumWhiptail 0x3d

enum PLATINUM_Commands
{
    DeviceID = 0x00,
    FirmwareID = 0x01,
    ProductName = 0x02,
    Status = 0x03,
    LED_SelectCurrent = 0x04,
    LED_Count = 0x05,
    LED_Mode = 0x06,
    LED_CurrentColor = 0x07,
    LED_TemperatureColor = 0x08,
    LED_TemperatureMode = 0x09,
    LED_TemperatureModeColors = 0x0A,
    LED_CycleColors = 0x0B,
    TEMP_SelectActiveSensor = 0x0C,
    TEMP_CountSensors = 0x0D,
    TEMP_Read = 0x0E,
    TEMP_Limit = 0x0F,
    FAN_Select = 0x10,
    FAN_Count = 0x11,
    FAN_Mode = 0x12,
    FAN_FixedPWM = 0x13,
    FAN_FixedRPM = 0x14,
    FAN_ReportExtTemp = 0x15,
    FAN_ReadRPM = 0x16,
    FAN_MaxRecordedRPM = 0x17,
    FAN_UnderSpeedThreshold = 0x18,
    FAN_RPMTable = 0x19,
    FAN_TempTable = 0x1A
};

enum PLATINUM_Op_Codes
{
    WriteOneByte = 0x06,
    ReadOneByte = 0x07,
    WriteTwoBytes = 0x08,
    ReadTwoBytes = 0x09,
    WriteThreeBytes = 0x0A,
    ReadThreeBytes = 0x0B,
};

enum PLATINUM_Led_Modes
{
    StaticColor = 0x00,
    TwoColorCycle = 0x40,
    FourColorCycle = 0x80,
    TemperatureColor = 0xC0
};

enum PLATINUM_Fan_Modes
{
    PLATINUM_FixedPWM = 0x02,
    PLATINUM_FixedRPM = 0x04,
    PLATINUM_Default = 0x06,
    PLATINUM_Quiet = 0x08,
    PLATINUM_Balanced = 0x0A,
    PLATINUM_Performance = 0x0C,
    PLATINUM_Custom = 0x0E
};

static uint8_t CommandId = 0x00;

#define PLATINUM_FAN_TABLE_QUIET( x ) \
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

#define PLATINUM_FAN_TABLE_BALANCED( x ) \
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

#define PLATINUM_FAN_TABLE_EXTREME( x ) \
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
corsairlink_platinum_device_id(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint16_t* device_id );

int
corsairlink_platinum_name(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_size );

int
corsairlink_platinum_vendor(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_size );

int
corsairlink_platinum_product(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_size );

int
corsairlink_platinum_firmware_id(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* firmware,
    uint8_t firmware_size );

int
corsairlink_platinum_change_led(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct led_control* ctrl );

/* Temperature */
int
corsairlink_platinum_temperature(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t selector,
    double* temperature );

int
corsairlink_platinum_tempsensorscount(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t* temperature_sensors_count );

/* Fan */
int
corsairlink_platinum_fan_count(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_platinum_fan_mode_read(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_platinum_fan_mode_read_rpm(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_platinum_fan_mode_read_pwm(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_platinum_fan_mode_default(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_platinum_fan_mode_performance(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_platinum_fan_mode_balanced(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_platinum_fan_mode_quiet(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_platinum_fan_mode_rpm(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_platinum_fan_mode_pwm(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_platinum_fan_mode_custom(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_platinum_fan_curve(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_platinum_fan_speed(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_platinum_fan_print_mode(
    uint8_t mode, uint16_t data, char* modestr, uint8_t modestr_size );

/* Pump */
int
corsairlink_platinum_pump_mode(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

int
corsairlink_platinum_pump_mode_read(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

int
corsairlink_platinum_pump_mode_default(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

int
corsairlink_platinum_pump_mode_quiet(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

int
corsairlink_platinum_pump_mode_balanced(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

int
corsairlink_platinum_pump_mode_performance(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

int
corsairlink_platinum_pump_mode_custom(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

int
corsairlink_platinum_pump_curve(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

int
corsairlink_platinum_pump_speed(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

#endif
