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

#ifndef _SCAN_H
#define _SCAN_H

#include <libusb.h>

struct corsair_device_scan
{
    struct corsair_device_info* device;

    /** libusb device structures */
    struct libusb_device_handle* handle;
} scanlist[10];

int
corsairlink_handle_close( struct libusb_device_handle* handle );
int
corsairlink_close( libusb_context* context );
int
corsairlink_device_scanner( libusb_context* context, int* _scanlist_count );

#endif
