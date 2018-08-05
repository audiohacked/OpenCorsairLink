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

/*! \file lowlevel/asetek.c
 *  \brief Lowlevel Routines for Asetek
 */
#include "lowlevel/asetek.h"

#include <libusb.h>
#include <stdio.h>

#define TIMEOUT_DEFAULT 1000 /*!< TIMEOUT_DEFAULT */

/*! Asetek Init routine.
 *  Asetek init routine uses two control transfers.
 *
 */
int
corsairlink_asetek_init( struct libusb_device_handle* dev_handle, uint8_t endpoint )
{
    int rr;

    rr = libusb_control_transfer( dev_handle, 0x40, 0x00, 0xffff, 0x0000, NULL, 0, 0 );
    rr = libusb_control_transfer( dev_handle, 0x40, 0x02, 0x0002, 0x0000, NULL, 0, 0 );

    return rr;
}

/*! Asetek De-Init routine.
 *  Asetek de-init routine uses a control transfer.
 *
 */
int
corsairlink_asetek_deinit( struct libusb_device_handle* dev_handle, uint8_t endpoint )
{
    int rr;

    rr = libusb_control_transfer( dev_handle, 0x40, 0x02, 0x0004, 0x0000, NULL, 0, 200 );

    return rr;
}

/*! Lowlevel Asetek Write routine.
 *  Lowlevel Asetek Write rotine uses bulk transfer to send commands.
 *
 */
int
corsairlink_asetek_write(
    struct libusb_device_handle* dev_handle, uint8_t endpoint, uint8_t* data, int length )
{
    int bytes_transferred;
    int rr;

    rr = libusb_bulk_transfer(
        dev_handle, endpoint, data, length, &bytes_transferred, TIMEOUT_DEFAULT );

    return rr;
}

/*! Lowlevel Asetek Read routine.
 *  Lowlevel Asetek Read routine uses bulk transfer to receive responses.
 *
 */
int
corsairlink_asetek_read(
    struct libusb_device_handle* dev_handle, uint8_t endpoint, uint8_t* data, int length )
{
    int bytes_transferred;
    int rr;

    rr = libusb_bulk_transfer(
        dev_handle, endpoint, data, length, &bytes_transferred, TIMEOUT_DEFAULT );

    return rr;
}
