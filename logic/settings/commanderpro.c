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
commanderpro_settings(
    struct corsair_device_scan scanned_device,
    struct option_flags flags,
    struct option_parse_return settings )
{
    int rr;
    int ii;
    char name[32];
    name[sizeof( name ) - 1] = 0;
    double output_volts;
    uint32_t time = 0;
    struct corsair_device_info* dev;
    struct libusb_device_handle* handle;

    dev = scanned_device.device;
    handle = scanned_device.handle;
    msg_debug( "DEBUG: shortcuts set\n" );

    rr = dev->driver->init( handle, dev->write_endpoint );
    msg_debug( "DEBUG: init done\n" );

    /* fetch device name, vendor name, product name */
    rr = dev->driver->name( dev, handle, name, sizeof( name ) );
    rr = dev->driver->vendor( dev, handle, name, sizeof( name ) );
    msg_info( "Vendor: %s\n", name );
    rr = dev->driver->product( dev, handle, name, sizeof( name ) );
    msg_info( "Product: %s\n", name );
    rr = dev->driver->fw_version( dev, handle, name, sizeof( name ) );
    msg_info( "Firmware: %s\n", name );
    msg_debug( "DEBUG: string done\n" );

    /* fetch temperatures */
    for ( ii = 0; ii < 4; ii++ )
    {
        // char temperature[16];
        double temperature;
        rr = dev->driver->temperature.read( dev, handle, ii, &temperature );
        msg_info( "Temperature %d: %5.2f C\n", ii, temperature );
    }

    /* fetch SATA voltages */
    for ( ii = 0; ii < 3; ii++ )
    {
        if ( ii == 0 )
            msg_info( "Output 12v: " );
        if ( ii == 1 )
            msg_info( "Output 5v: " );
        if ( ii == 2 )
            msg_info( "Output 3.3v: " );

        rr = dev->driver->power.voltage( dev, handle, ii, &output_volts );
        msg_info( "%5.2f V\n", output_volts );
    }

    msg_debug( "Setting LED\n" );
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

    rr = dev->driver->deinit( handle, dev->write_endpoint );

    return 0;
}
