/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017-2020  Sean Nelson <audiohacked@gmail.com>

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

#include "logic/scan.h"

#include "common.h"
#include "device.h"
#include "driver.h"
#include "logic/print.h"

#include <errno.h>
#include <getopt.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int scanlist_count = 0;
struct corsair_device_scan scanlist[10];

int
corsairlink_handle_close( struct libusb_device_handle* handle )
{
    int rr;

    rr = libusb_release_interface( handle, 0 );
    if ( rr < 0 )
    {
        msg_err( "Encountered LibUSB Error: %s!\n", libusb_strerror( rr ) );
    }

    rr = libusb_attach_kernel_driver( handle, 0 );
    if ( rr < 0 )
    {
        msg_err( "Encountered LibUSB Error: %s!\n", libusb_strerror( rr ) );
    }

    libusb_close( handle );

    return rr;
}

int
corsairlink_close( libusb_context* context )
{
    int rr;
    int ii;

    for ( ii = 0; ii < scanlist_count; ii++ )
    {
        rr = corsairlink_handle_close( scanlist[ii].handle );
    }
    libusb_exit( context );

    return 0;
}

int
corsairlink_check_device_id( struct corsair_device_info* cl_device )
{
    int rr;

    /* get device_id if we have a proper device handle */
    uint8_t device_id = 0x00;
    cl_device->driver->device_id( cl_device, scanlist[scanlist_count].handle, &device_id );

    /* check to see if the device_id is the right one */
    if ( cl_device->device_id == device_id )
    {
        /* if we have the right device id we can setup the rest
         * of the device connections
         */
        scanlist[scanlist_count].device = cl_device;
        msg_info( "Dev=%d, CorsairLink Device Found: %s!\n", scanlist_count, cl_device->name );
        scanlist_count++;
        return 1; // break;

    }
    else
    {
        msg_debug( "No (device_id 0x%02X)\n", device_id );
        rr = corsairlink_handle_close( scanlist[scanlist_count].handle );
        return 2; // continue;
    }
}

int
corsairlink_device_setup(
    libusb_device* device,
    struct corsair_device_info* cl_device,
    struct libusb_device_descriptor desc )
{
    int rr; // This could be safely ignored. It is just a success flag for
            // libusb functions.

    if ( ( cl_device->vendor_id == desc.idVendor ) 
        && ( cl_device->product_id == desc.idProduct ) )
    {
        msg_debug( "Corsair product detected. Checking if device is %s... ", cl_device->name );
        rr = libusb_open( device, &scanlist[scanlist_count].handle );
        if ( rr < 0 )
        {
            msg_err( "Encountered LibUSB Error: %s!\n", libusb_strerror( rr ) );
            return rr;
        }

        rr = libusb_kernel_driver_active( scanlist[scanlist_count].handle, 0 );
        if (rr == 1)
        {
            msg_debug("Corsair Device has kernel driver attached\n");

            rr = libusb_detach_kernel_driver( scanlist[scanlist_count].handle, 0 );
            if ( rr < 0 )
            {
                msg_debug("Encountered LibUSB Error: %s!\n", libusb_strerror( rr ) );
                return rr;
            }
            else
            {
                msg_debug("Detached kernel driver from Corsair Device\n");
            }

        }
        else if ( rr == 0 )
        {
            msg_debug("Corsair Device has no kernel driver attached\n");
        }
        else
        {
            msg_debug("Encountered LibUSB Error: %s!\n", libusb_strerror( rr ) );
            return rr;
        }

        // libusb_reset_device()
        // libusb_get_device()
        // libusb_set_configuration()

        rr = libusb_claim_interface( scanlist[scanlist_count].handle, 0 );
        if ( rr < 0 )
        {
            msg_debug("Encountered LibUSB Error: %s!\n", libusb_strerror( rr ) );
            return rr;
        }
        else
        {
            msg_debug("Claimed Interface on Corsair Device\n");
        }

        // libusb_set_interface_alt_setting
        // libusb_get_device_descriptor
        // libusb_get_bus_number
        // libusb_get_device_address
        // libusb_get_string_descriptor_ascii

        return corsairlink_check_device_id( cl_device );
    }

    return 0;
}

int
corsairlink_device_enumerate( libusb_device* devices )
{
    int rr; // This could be safely ignored. It is just a success flag for
            // libusb functions.
    int jj; // Loops through known CorsairLink Devices.

    if ( scanlist_count >= 10 )
    {
        msg_debug( "Limited to 10 CorsairLink devices\n" );
        return 1; // break;
    }

    struct libusb_device_descriptor desc;
    rr = libusb_get_device_descriptor( devices, &desc );
    if ( rr < 0 )
    {
        msg_err( "Encountered LibUSB Error: %s!\n", libusb_strerror( rr ) );
        return rr;
    }

    // msg_debug( "Checking USB device %d (%04x:%04x)...\n", ii, desc.idVendor, desc.idProduct );
    msg_debug( "Checking USB device (%04x:%04x)...\n", desc.idVendor, desc.idProduct );

    for ( jj = 0; jj < corsairlink_device_list_count; jj++ )
    {
        rr = corsairlink_device_setup( devices, &corsairlink_devices[jj], desc );
        if ( rr == 1 )
        {
            break;
        }
        else if (rr == 2 )
        {
            continue;
        }
        // return rr;
    }

    return 0;
}

int
corsairlink_device_scanner( libusb_context* context, int* _scanlist_count )
{
    int rr; // This could be safely ignored. It is just a success flag for
            // libusb functions.

    /* Start: scan code */
    int ii; // Loops through USB devices.
    ssize_t cnt;
    libusb_device** devices;
    // uint16_t device_id = 0x00;
    // uint16_t firmware_id = 0x0000;

    cnt = libusb_get_device_list( context, &devices );
    for ( ii = 0; ii < cnt; ii++ )
    {
        rr = corsairlink_device_enumerate( devices[ii] );
        if ( rr == 1 )
        {
            break;
        }
    }
    msg_info( "\n" );
    *_scanlist_count = scanlist_count;
    /* End: scan code */

    return 0;
}
