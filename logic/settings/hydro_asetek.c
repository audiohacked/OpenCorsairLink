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
#include "logic/options.h"
#include "logic/scan.h"
#include "print.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
hydro_asetek_settings(
    struct corsair_device_scan scanned_device,
    struct option_flags flags,
    struct option_parse_return settings )
{
    int rr;
    int ii;
    char name[20];
    memset( name, '\0', sizeof( name ) );
    char fan_mode_string[64];
    memset( fan_mode_string, '\0', sizeof( fan_mode_string ) );
    struct corsair_device_info* dev;
    struct libusb_device_handle* handle;

    uint8_t temperature_sensors_count = 0;
    uint16_t temperature;
    double celsius = 0;

    enum motor_modes pump_mode = UNDEFINED;
    uint16_t pump_speed = 0;
    uint16_t pump_max_speed = 0;

    enum motor_modes fan_mode = UNDEFINED;
    uint8_t fan_count = 0;
    uint8_t fan_index = 0;
    uint16_t fan_data = 0;
    uint16_t fan_speed = 0;
    uint16_t fan_max_speed = 0;

    dev = scanned_device.device;
    handle = scanned_device.handle;
    msg_debug( "DEBUG: shortcuts set\n" );

    rr = dev->driver->init( handle, dev->write_endpoint );
    msg_debug( "DEBUG: init done\n" );

    /* fetch device name, vendor name, product name */
    rr = dev->driver->vendor( dev, handle, name, sizeof( name ) );
    msg_info( "Vendor: %s\n", name );
    rr = dev->driver->product( dev, handle, name, sizeof( name ) );
    msg_info( "Product: %s\n", name );
    rr = dev->driver->fw_version( dev, handle, name, sizeof( name ) );
    msg_info( "Firmware: %s\n", name );

    /* get number of temperature sensors */
    rr = dev->driver->temperature.count( dev, handle, &temperature_sensors_count );

    for ( ii = 0; ii < temperature_sensors_count; ii++ )
    {
        double temperature;
        rr = dev->driver->temperature.read( dev, handle, ii, &temperature );
        msg_info( "Temperature %d: %5.2f C\n", ii, temperature );
        msg_machine( "temperature:%d:%5.2f\n", ii, temperature );
    }

    /* read fans info */
    rr = dev->driver->fan.count( dev, handle, &settings.fan_ctrl );
    for ( ii = 0; ii < settings.fan_ctrl.fan_count; ii++ )
    {
        settings.fan_ctrl.channel = ii;
        rr = dev->driver->fan.profile.read_profile( dev, handle, &settings.fan_ctrl );
        rr = dev->driver->fan.print_mode(
            settings.fan_ctrl.mode, settings.fan_ctrl.data, settings.fan_ctrl.mode_string,
            sizeof( settings.fan_ctrl.mode_string ) );
        rr = dev->driver->fan.speed( dev, handle, &settings.fan_ctrl );
        msg_info( "Fan %d:\t%s\n", ii, settings.fan_ctrl.mode_string );
        msg_info(
            "\tCurrent/Max Speed %i/%i RPM\n", settings.fan_ctrl.speed_rpm,
            settings.fan_ctrl.max_speed );
        msg_machine(
            "fan:%d:%d:%i:%i\n", ii, settings.fan_ctrl.mode, settings.fan_ctrl.speed_rpm,
            settings.fan_ctrl.max_speed );
    }

    /* read pump info */
    rr = dev->driver->pump.profile.read( dev, handle, &settings.pump_ctrl );
    rr = dev->driver->pump.speed( dev, handle, &settings.pump_ctrl );
    msg_info( "Pump:\tMode 0x%02X\n", settings.pump_ctrl.mode );
    msg_info(
        "\tCurrent/Max Speed %i/%i RPM\n", settings.pump_ctrl.speed, settings.pump_ctrl.max_speed );
    msg_machine(
        "pump:%d:%i:%i\n", settings.pump_ctrl.mode, settings.pump_ctrl.speed,
        settings.pump_ctrl.max_speed );

    if ( flags.set_led == 1 )
    {
        msg_debug( "Setting LED Flag found\n" );
        switch ( settings.led_ctrl.mode )
        {
        case BLINK:
            msg_debug( "Setting LED to BLINK\n" );
            if ( dev->driver->led.blink != NULL )
            {
                rr = dev->driver->led.blink( dev, handle, &settings.led_ctrl );
            }
            break;
        case PULSE:
            msg_debug( "Setting LED to PULSE\n" );
            if ( dev->driver->led.color_pulse != NULL )
            {
                rr = dev->driver->led.color_pulse( dev, handle, &settings.led_ctrl );
            }
            break;
        case SHIFT:
            msg_debug( "Setting LED to SHIFT\n" );
            if ( dev->driver->led.color_shift != NULL )
            {
                rr = dev->driver->led.color_shift( dev, handle, &settings.led_ctrl );
            }
            break;
        case RAINBOW:
            msg_debug( "Setting LED to RAINBOW\n" );
            if ( dev->driver->led.rainbow != NULL )
            {
                rr = dev->driver->led.rainbow( dev, handle, &settings.led_ctrl );
            }
            break;
        case TEMPERATURE:
            msg_debug( "Setting LED to TEMPERATURE\n" );
            if ( dev->driver->led.temperature != NULL )
            {
                rr = dev->driver->led.temperature( dev, handle, &settings.led_ctrl );
            }
            break;
        case STATIC:
        default:
            msg_debug( "Setting LED STATIC\n" );
            if ( dev->driver->led.static_color != NULL )
            {
                rr = dev->driver->led.static_color( dev, handle, &settings.led_ctrl );
            }
            break;
        }
    }

    if ( flags.set_fan == 1 )
    {
        switch ( settings.fan_ctrl.mode )
        {
        case QUIET:
            if ( dev->driver->fan.profile.write_profile_quiet != NULL )
            {
                dev->driver->fan.profile.write_profile_quiet( dev, handle, &settings.fan_ctrl );
            }
            break;
        case BALANCED:
            if ( dev->driver->fan.profile.write_profile_balanced != NULL )
            {
                dev->driver->fan.profile.write_profile_balanced( dev, handle, &settings.fan_ctrl );
            }
            break;
        case PERFORMANCE:
            if ( dev->driver->fan.profile.write_profile_performance != NULL )
            {
                dev->driver->fan.profile.write_profile_performance(
                    dev, handle, &settings.fan_ctrl );
            }
            break;
        case CUSTOM:
            if ( dev->driver->fan.profile.write_custom_curve != NULL )
            {
                dev->driver->fan.profile.write_custom_curve( dev, handle, &settings.fan_ctrl );
            }
            break;
        default:
            msg_info( "Unsupported Fan Mode\n" );
            break;
        }
    }

    if ( flags.set_pump == 1 )
    {
        settings.pump_ctrl.channel = dev->pump_index;
        switch ( settings.pump_ctrl.mode )
        {
        case QUIET:
        case BALANCED:
        case PERFORMANCE:
        case CUSTOM:
        default:
            dev->driver->pump.profile.custom( dev, handle, &settings.pump_ctrl );
            break;
        }
    }

    rr = dev->driver->deinit( handle, dev->write_endpoint );
    msg_debug( "DEBUG: deinit done\n" );

    return 0;
}
