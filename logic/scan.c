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

#include "logic/scan.h"

#include "common.h"
#include "device.h"
#include "driver.h"
#include "print.h"

#include <errno.h>
#include <getopt.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int scanlist_count = 0;

int
corsairlink_handle_close( struct libusb_device_handle* handle )
{
    int rr;

    rr = libusb_release_interface( handle, 0 );
    rr = libusb_attach_kernel_driver( handle, 0 );
    libusb_close( handle );

    return rr;
}

int
corsairlink_close( libusb_context* context )
{
    int ii;

    for ( ii = 0; ii < scanlist_count; ii++ )
    {
        corsairlink_handle_close( scanlist[ii].handle );
    }
    libusb_exit( context );

    return 0;
}

int
corsairlink_device_scanner( libusb_context* context, int* _scanlist_count )
{
    int rr; // This could be safely ignored. It is just a success flag for
            // libusb functions.

    /* Start: scan code */
    int ii; // Loops through USB devices.
    int jj; // Loops through known CorsairLink Devices.
    ssize_t cnt;
    struct corsair_device_info* device;
    libusb_device** devices;
    uint8_t device_id = 0x00;
    // uint16_t firmware_id = 0x0000;

    cnt = libusb_get_device_list( context, &devices );
    for ( ii = 0; ii < cnt; ii++ )
    {
        if ( scanlist_count >= 10 )
        {
            msg_debug( "Limited to 10 CorsairLink devices\n" );
            break;
        }

        struct libusb_device_descriptor desc;
        rr = libusb_get_device_descriptor( devices[ii], &desc );
        msg_debug( "Checking USB device %d (%04x:%04x)...\n", ii, desc.idVendor, desc.idProduct );

        for ( jj = 0; jj < corsairlink_device_list_count; jj++ )
        {
            device = &corsairlink_devices[jj];

            if ( ( device->vendor_id == desc.idVendor )
                 && ( device->product_id == desc.idProduct ) )
            {
                msg_debug( "Corsair product detected. Checking if device is %s... ", device->name );
                rr = libusb_open( devices[ii], &scanlist[scanlist_count].handle );
                if ( scanlist[scanlist_count].handle != NULL )
                { // Maybe try 'if (rr == 0)'
                    rr = libusb_detach_kernel_driver( scanlist[scanlist_count].handle, 0 );
                    rr = libusb_claim_interface( scanlist[scanlist_count].handle, 0 );

                    /* get device_id if we have a proper device handle */
                    device->driver->device_id(
                        device, scanlist[scanlist_count].handle, &device_id );
                    /* check to see if the device_id is the right one */
                    if ( device->device_id == device_id )
                    {
                        /* if we have the right device id we can setup the rest
                         * of the device connections
                         */
                        scanlist[scanlist_count].device = device;
                        msg_info(
                            "Dev=%d, CorsairLink Device Found: %s!\n", scanlist_count,
                            device->name );
                        scanlist_count++;
                        break;
                    }
                    else
                    {
                        msg_debug( "No (device_id 0x%02X)\n", device_id );

                        corsairlink_handle_close( scanlist[scanlist_count].handle );
                        continue;
                    }
                }
                else
                {
                    msg_debug( "Could not open device %d:%d.", desc.idVendor, desc.idProduct );
                }
            }
        }
    }
    msg_info( "\n" );
    *_scanlist_count = scanlist_count;
    /* End: scan code */

    return 0;
}
