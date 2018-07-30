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

int hydro_coolit_settings(
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
    uint16_t temperature;
    uint8_t temperature_sensors_count = 0;
    uint8_t fan_count = 0;
    double celsius = 0;
    uint8_t pump_mode = 0;
    uint16_t pump_speed = 0;
    uint16_t pump_max_speed = 0;
    uint8_t fan_index = 0;
    uint8_t fan_mode = UNDEFINED;
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
    rr = dev->driver->tempsensorscount(
        dev, handle, &temperature_sensors_count );

    for ( ii = 0; ii < temperature_sensors_count; ii++ )
    {
        // char temperature[10];
        double temperature;
        rr = dev->driver->temperature( dev, handle, ii, &temperature );
        msg_info( "Temperature %d: %5.2f C\n", ii, temperature );
    }

    /* get number of fans */
    rr = dev->driver->fan.count( dev, handle, &fan_count );

    for ( ii = 0; ii < fan_count; ii++ )
    {
        // fan_mode = UNDEFINED;
        // fan_speed = 0;
        // fan_max_speed = 0;
        // fan_data = 0;
        rr = dev->driver->fan.profile.read(
            dev, handle, ii, &fan_mode, &fan_data );
        rr = dev->driver->fan.print_mode(
            fan_mode, fan_data, fan_mode_string, sizeof( fan_mode_string ) );
        rr = dev->driver->fan.speed(
            dev, handle, ii, &fan_speed, &fan_max_speed );
        msg_info( "Fan %d:\t%s\n", ii, fan_mode_string );
        msg_info( "\tCurrent/Max Speed %i/%i RPM\n", fan_speed, fan_max_speed );
    }

    rr = dev->driver->pump.profile( dev, handle, &pump_mode );
    rr = dev->driver->pump.speed( dev, handle, &pump_speed, &pump_max_speed );

    msg_info( "Pump:\tMode 0x%02X\n", pump_mode );
    msg_info( "\tCurrent/Max Speed %i/%i RPM\n", pump_speed, pump_max_speed );

    if ( flags.set_led == 1 )
    {
        msg_debug( "Setting LED Flag found\n" );
        switch ( settings.led_ctrl.mode )
        {
        case STATIC:
            msg_debug( "Setting LED STATIC\n" );
        default:
            msg_debug( "Setting LED DEFAULT\n" );
            rr = dev->driver->led.static_color(
                dev, handle, &settings.led_ctrl );
            break;
        }
    }

    if ( flags.set_pump && settings.pump_ctrl.mode != DEFAULT )
    {
        msg_info( "Setting pump to mode: %i\n", settings.pump_ctrl.mode );
        rr = dev->driver->pump.profile( dev, handle, &settings.pump_ctrl.mode );
        pump_mode = 0;
        pump_speed = 0;
        pump_max_speed = 0;
        rr = dev->driver->pump.profile( dev, handle, &pump_mode );
        // sleep 3 seconds for pump to reach new value
        sleep( 3 );
        rr = dev->driver->pump.speed(
            dev, handle, &pump_speed, &pump_max_speed );
        msg_info( "Pump:\tMode 0x%02X\n", pump_mode );
        msg_info(
            "\tCurrent/Max Speed %i/%i RPM\n", pump_speed, pump_max_speed );
    }
    if ( settings.fan_ctrl.channel > 0
         && settings.fan_ctrl.channel < fan_count + 1 )
    {
        fan_mode = settings.fan_ctrl.mode;
        fan_data = settings.fan_ctrl.data;
        rr = dev->driver->fan.profile.balanced(
            dev, handle, settings.fan_ctrl.channel - 1, &fan_mode, &fan_data );
    }

    rr = dev->driver->deinit( handle, dev->write_endpoint );
    msg_debug( "DEBUG: deinit done\n" );

    return 0;
}
