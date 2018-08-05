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
psu_settings(
    struct corsair_device_scan scanned_device,
    struct option_flags flags,
    struct option_parse_return settings )
{
    int rr;
    int ii;
    char name[32];
    name[sizeof( name ) - 1] = 0;
    uint32_t time = 0;
    double supply_volts, supply_watts, temperature, output_volts, output_amps, output_watts;
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
    for ( ii = 0; ii < 2; ii++ )
    {
        rr = dev->driver->temperature.read( dev, handle, ii, &temperature );
        msg_info( "Temperature %d: %5.2f C\n", ii, temperature );
    }

    /* fetch device powered time and device uptime */
    rr = dev->driver->psu_time.powered( dev, handle, &time );
    msg_info( "Powered: %u (%dd.  %dh)\n", time, time / ( 24 * 60 * 60 ), time / ( 60 * 60 ) % 24 );
    rr = dev->driver->psu_time.uptime( dev, handle, &time );
    msg_info( "Uptime: %u (%dd.  %dh)\n", time, time / ( 24 * 60 * 60 ), time / ( 60 * 60 ) % 24 );
    msg_debug( "DEBUG: time done\n" );

    /* fetch Supply Voltage and Total Watts Consumming */
    rr = dev->driver->power.supply_voltage( dev, handle, &supply_volts );
    msg_info( "Supply Voltage: %5.2f V\n", supply_volts );
    rr = dev->driver->power.total_wattage( dev, handle, &supply_watts );
    msg_info( "Total Watts: %5.2f W\n", supply_watts );
    msg_debug( "DEBUG: supply done\n" );

    /* fetch PSU output */
    for ( ii = 0; ii < 3; ii++ )
    {
        if ( ii == 0 )
            msg_info( "Output 12v:\n" );
        if ( ii == 1 )
            msg_info( "Output 5v:\n" );
        if ( ii == 2 )
            msg_info( "Output 3.3v:\n" );

        rr = dev->driver->power.sensor_select( dev, handle, ii );
        rr = dev->driver->power.voltage( dev, handle, ii, &output_volts );
        msg_info( "\tVoltage %5.2f V\n", output_volts );

        rr = dev->driver->power.amperage( dev, handle, ii, &output_amps );
        msg_info( "\tAmps %5.2f A\n", output_amps );

        rr = dev->driver->power.wattage( dev, handle, ii, &output_watts );
        msg_info( "\tWatts %5.2f W\n", output_watts );
    }
    rr = dev->driver->power.sensor_select( dev, handle, 0 );

    rr = dev->driver->deinit( handle, dev->write_endpoint );

    return 0;
}
