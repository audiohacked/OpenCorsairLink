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
#include "protocol/asetek.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
hydro_asetekpro_settings(
    struct corsair_device_scan scanned_device,
    struct option_flags flags,
    struct option_parse_return settings )
{
    int rr;
    int ii;
    char name[20];
    memset( name, '\0', sizeof( name ) );
    struct corsair_device_info* dev;
    struct libusb_device_handle* handle;
    struct option_parse_return readings = settings;
    uint8_t temperature_sensors_count = 0;

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
        // char temperature[10];
        double temperature;
        rr = dev->driver->temperature.read( dev, handle, ii, &temperature );
        msg_info( "Temperature %d: %5.2f C\n", ii, temperature );
    }

    /* get number of fans */
    rr = dev->driver->fan.count( dev, handle, &readings.fan_ctrl );

    for ( ii = 0; ii < readings.fan_ctrl.fan_count; ii++ )
    {
        readings.fan_ctrl.channel = ii;
        rr = dev->driver->fan.profile.read_profile( dev, handle, &readings.fan_ctrl );
        rr = dev->driver->fan.print_mode(
            readings.fan_ctrl.mode, readings.fan_ctrl.data, readings.fan_ctrl.mode_string,
            sizeof( readings.fan_ctrl.mode_string ) );
        rr = dev->driver->fan.speed( dev, handle, &readings.fan_ctrl );
        msg_info( "Fan %d:\t%s\n", ii, readings.fan_ctrl.mode_string );
        msg_info(
            "\tCurrent/Max Speed %i/%i RPM\n", readings.fan_ctrl.speed_rpm,
            settings.fan_ctrl.max_speed );
    }

    rr = dev->driver->pump.profile.read_profile( dev, handle, &readings.pump_ctrl );
    rr = dev->driver->pump.speed( dev, handle, &readings.pump_ctrl );
    msg_info( "Pump:\tMode 0x%02X\n", readings.pump_ctrl.mode );
    msg_info(
        "\tCurrent/Max Speed %i/%i RPM\n", readings.pump_ctrl.speed, readings.pump_ctrl.max_speed );

    if ( flags.set_led == 1 )
    {
        msg_debug( "Setting LED Flag found\n" );
        switch ( settings.led_ctrl.mode )
        {
        case BLINK:
            msg_debug( "Setting LED to BLINK\n" );
            rr = dev->driver->led.blink( dev, handle, &settings.led_ctrl );
            break;
        case PULSE:
            msg_debug( "Setting LED to PULSE\n" );
            rr = dev->driver->led.color_pulse( dev, handle, &settings.led_ctrl );
            break;
        case SHIFT:
            msg_debug( "Setting LED to SHIFT\n" );
            rr = dev->driver->led.color_shift( dev, handle, &settings.led_ctrl );
            break;
        case RAINBOW:
            msg_debug( "Setting LED to RAINBOW\n" );
            rr = dev->driver->led.rainbow( dev, handle, &settings.led_ctrl );
            break;
        case TEMPERATURE:
            msg_debug( "Setting LED to TEMPERATURE\n" );
            rr = dev->driver->led.temperature( dev, handle, &settings.led_ctrl );
            break;
        case STATIC:
        default:
            msg_debug( "Setting LED STATIC\n" );
            rr = dev->driver->led.static_color( dev, handle, &settings.led_ctrl );
            break;
        }
    }

    if ( flags.set_fan == 1 )
    {
        msg_info( "Setting fan to mode: %u\n", settings.fan_ctrl.mode );
        switch ( settings.fan_ctrl.mode )
        {
        case PWM:
            dev->driver->fan.profile.write_pwm( dev, handle, &settings.fan_ctrl );
            break;
        case RPM:
            dev->driver->fan.profile.write_rpm( dev, handle, &settings.fan_ctrl );
            break;
        case QUIET:
            dev->driver->fan.profile.write_profile_quiet( dev, handle, &settings.fan_ctrl );
            break;
        case DEFAULT:
        case BALANCED:
            dev->driver->fan.profile.write_profile_balanced( dev, handle, &settings.fan_ctrl );
            break;
        case PERFORMANCE:
            dev->driver->fan.profile.write_profile_performance( dev, handle, &settings.fan_ctrl );
            break;
        case CUSTOM:
            dev->driver->fan.profile.write_custom_curve( dev, handle, &settings.fan_ctrl );
            break;
        default:
            msg_info( "Unsupported AsetekPro Fan Mode\n" );
            break;
        }
    }

    if ( flags.set_pump == 1 )
    {
        msg_info( "Setting pump to mode: %u\n", settings.pump_ctrl.mode );
        settings.pump_ctrl.channel = dev->pump_index;
        switch ( settings.pump_ctrl.mode )
        {
        case QUIET:
            ASETEK_FAN_TABLE_QUIET( settings.pump_ctrl.table );
            dev->driver->pump.profile.write_custom_curve( dev, handle, &settings.pump_ctrl );
            break;
        case BALANCED:
            ASETEK_FAN_TABLE_BALANCED( settings.pump_ctrl.table );
            dev->driver->pump.profile.write_custom_curve( dev, handle, &settings.pump_ctrl );
            break;
        case PERFORMANCE:
            ASETEK_FAN_TABLE_EXTREME( settings.pump_ctrl.table );
            dev->driver->pump.profile.write_custom_curve( dev, handle, &settings.pump_ctrl );
            break;
        case CUSTOM:
        default:
            dev->driver->pump.profile.write_custom_curve( dev, handle, &settings.pump_ctrl );
            break;
        }
    }

    rr = dev->driver->deinit( handle, dev->write_endpoint );
    msg_debug( "DEBUG: deinit done\n" );

    return 0;
}
