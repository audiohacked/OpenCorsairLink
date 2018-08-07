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

/*! \file protocol/flextronics/core.h
 *  \brief Core Routines for Flextronics Series of Power Supplies
 */
#ifndef _PROTOCOL_FLEXTRONICS_H
#define _PROTOCOL_FLEXTRONICS_H

enum Action
{
    READ_VERSION = 0x00,
    READ_MEMORY = 0x08,
    WRITE_SMB_SETTINGS = 0x11,
    READ_SMB_COMMAND = 0x12,
    WRITE_SMB_COMMAND = 0x13,
};

double
convert_bytes_double( uint16_t v16 );

int
corsairlink_flextronics_device_id(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint8_t* device_id );

int
corsairlink_flextronics_firmware_id(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* firmware,
    uint8_t firmware_size );

int
corsairlink_flextronics_channel_id( struct corsair_device_info* dev );

int
corsairlink_flextronics_product_id( struct corsair_device_info* dev );

int
corsairlink_flextronics_sensor_select(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint8_t sensor_select );

int
corsairlink_flextronics_output_volts(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t sensor_select,
    double* volts );

int
corsairlink_flextronics_output_amps(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t sensor_select,
    double* amps );

int
corsairlink_flextronics_output_watts(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t sensor_select,
    double* watts );

int
corsairlink_flextronics_power_total_wattage(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, double* watts );

int
corsairlink_flextronics_power_supply_voltage(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, double* volts );

int
corsairlink_flextronics_temperature(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    uint8_t sensor_select,
    double* temp );

int
corsairlink_flextronics_fan_mode(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint16_t* mode );

int
corsairlink_flextronics_fan_pwm(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint16_t* pwm );

int
corsairlink_flextronics_fan_pwm_percent(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint16_t* pwm_percent );

int
corsairlink_flextronics_fan_status(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint16_t* status );

int
corsairlink_flextronics_time_powered(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint32_t* v32 );

int
corsairlink_flextronics_time_uptime(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint32_t* v32 );

int
corsairlink_flextronics_name(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_size );

int
corsairlink_flextronics_vendor(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_size );

int
corsairlink_flextronics_product(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_size );

#endif
