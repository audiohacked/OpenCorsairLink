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

#ifndef _DRIVER_H
#define _DRIVER_H

#include "device.h"
#include "logic/options.h"

#include <libusb.h>

struct corsair_device_driver
{
    /** device communication helper functions */
    int ( *init )( struct libusb_device_handle*, uint8_t );
    int ( *deinit )( struct libusb_device_handle*, uint8_t );
    int ( *read )( struct libusb_device_handle*, uint8_t, uint8_t*, int );
    int ( *write )( struct libusb_device_handle*, uint8_t, uint8_t*, int );

    /** protocol functions */
    int ( *name )(
        struct corsair_device_info* dev,
        struct libusb_device_handle* handle,
        char* name,
        uint8_t name_size );

    int ( *vendor )(
        struct corsair_device_info* dev,
        struct libusb_device_handle* handle,
        char* name,
        uint8_t name_size );

    int ( *product )(
        struct corsair_device_info* dev,
        struct libusb_device_handle* handle,
        char* name,
        uint8_t name_size );

    int ( *device_id )(
        struct corsair_device_info* dev, struct libusb_device_handle* handle, uint8_t* device_id );

    int ( *fw_version )(
        struct corsair_device_info* dev,
        struct libusb_device_handle* handle,
        char* name,
        uint8_t name_size );

    struct temperature_functions
    {
        int ( *read )(
            struct corsair_device_info* dev,
            struct libusb_device_handle* handle,
            uint8_t sensor_select,
            double* temp );

        int ( *count )(
            struct corsair_device_info* dev,
            struct libusb_device_handle* handle,
            uint8_t* temperature_sensors_count );
    } temperature;

    struct led_functions
    {
        int ( *static_color )(
            struct corsair_device_info* dev,
            struct libusb_device_handle* handle,
            struct led_control* ctrl );

        int ( *blink )(
            struct corsair_device_info* dev,
            struct libusb_device_handle* handle,
            struct led_control* ctrl );

        int ( *color_pulse )(
            struct corsair_device_info* dev,
            struct libusb_device_handle* handle,
            struct led_control* ctrl );

        int ( *color_shift )(
            struct corsair_device_info* dev,
            struct libusb_device_handle* handle,
            struct led_control* ctrl );

        int ( *rainbow )(
            struct corsair_device_info* dev,
            struct libusb_device_handle* handle,
            struct led_control* ctrl );

        int ( *temperature )(
            struct corsair_device_info* dev,
            struct libusb_device_handle* handle,
            struct led_control* ctrl );
    } led;

    struct fan_functions
    {
        int ( *count )(
            struct corsair_device_info* dev,
            struct libusb_device_handle* handle,
            struct fan_control* ctrl );

        struct fan_profile_functions
        {
            int ( *read_profile )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct fan_control* ctrl );

            int ( *read_rpm )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct fan_control* ctrl );

            int ( *read_pwm )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct fan_control* ctrl );

            int ( *write_profile_default )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct fan_control* ctrl );

            int ( *write_profile_performance )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct fan_control* ctrl );

            int ( *write_profile_balanced )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct fan_control* ctrl );

            int ( *write_profile_quiet )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct fan_control* ctrl );

            int ( *write_profile_custom )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct fan_control* ctrl );

            int ( *write_rpm )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct fan_control* ctrl );

            int ( *write_pwm )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct fan_control* ctrl );

            int ( *write_custom_curve )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct fan_control* ctrl );
        } profile;

        int ( *speed )(
            struct corsair_device_info* dev,
            struct libusb_device_handle* handle,
            struct fan_control* ctrl );

        int ( *print_mode )( uint8_t mode, uint16_t data, char* modestr, uint8_t modestr_size );
    } fan;

    struct pump_functions
    {
        struct pump_profile_functions
        {
            int ( *read_profile )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct pump_control* ctrl );

            int ( *write_profile_default )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct pump_control* ctrl );

            int ( *write_profile_performance )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct pump_control* ctrl );

            int ( *write_profile_balanced )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct pump_control* ctrl );

            int ( *write_profile_quiet )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct pump_control* ctrl );

            int ( *write_profile_custom )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct pump_control* ctrl );

            int ( *write_custom_curve )(
                struct corsair_device_info* dev,
                struct libusb_device_handle* handle,
                struct pump_control* ctrl );
        } profile;

        int ( *speed )(
            struct corsair_device_info* dev,
            struct libusb_device_handle* handle,
            struct pump_control* ctrl );
    } pump;

    struct power_functions
    {
        int ( *supply_voltage )(
            struct corsair_device_info* dev, struct libusb_device_handle* handle, double* volts );

        int ( *total_wattage )(
            struct corsair_device_info* dev, struct libusb_device_handle* handle, double* watts );

        int ( *sensor_select )(
            struct corsair_device_info* dev,
            struct libusb_device_handle* handle,
            uint8_t sensor_select );

        int ( *voltage )(
            struct corsair_device_info* dev,
            struct libusb_device_handle* handle,
            uint8_t sensor_select,
            double* volts );

        int ( *amperage )(
            struct corsair_device_info* dev,
            struct libusb_device_handle* handle,
            uint8_t sensor_select,
            double* amps );

        int ( *wattage )(
            struct corsair_device_info* dev,
            struct libusb_device_handle* handle,
            uint8_t sensor_select,
            double* watts );
    } power;

    struct time_functions
    {
        int ( *powered )(
            struct corsair_device_info* dev, struct libusb_device_handle* handle, uint32_t* v32 );

        int ( *uptime )(
            struct corsair_device_info* dev, struct libusb_device_handle* handle, uint32_t* v32 );
    } psu_time;

    int ( *fan_speed_read )();
};

extern struct corsair_device_driver corsairlink_driver_asetek;
extern struct corsair_device_driver corsairlink_driver_asetekpro;
extern struct corsair_device_driver corsairlink_driver_commanderpro;
extern struct corsair_device_driver corsairlink_driver_dongle;
extern struct corsair_device_driver corsairlink_driver_coolit;
extern struct corsair_device_driver corsairlink_driver_rmi;

#endif
