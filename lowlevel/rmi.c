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

/*! \file lowlevel/rmi.c
 *  \brief Lowlevel Routines for RMi Series of Power Supplies
 */
#include "lowlevel/rmi.h"

#include <libusb.h>
#include <stdio.h>

#define TIMEOUT_DEFAULT 5000
#define INTERRUPT_IN_ENDPOINT 0x81
#define INTERRUPT_OUT_ENDPOINT 0x01

/*! RMi Power Supply Init
 *  RMi Power Supply Init is empty
 *  @param[in] handle for the data
 *  @param[in] device endpoint for the data
 *  @return 0
 */
int
corsairlink_rmi_init( struct libusb_device_handle* dev_handle, uint8_t endpoint )
{
    return 0;
}

/*! RMi Power Supply De-Init
 *  RMi Power Supply De-Init is empty
 *  @param[in] handle for the data
 *  @param[in] device endpoint for the data
 *  @return 0
 */
int
corsairlink_rmi_deinit( struct libusb_device_handle* dev_handle, uint8_t endpoint )
{
    return 0;
}

/*! RMi Power Supply Lowlevel Write
 *  RMi Power Supply Lowlevel Write uses a interrupt transfer
 *  @param[in] handle for the data
 *  @param[in] device endpoint for the data
 *  @param[in] data to send
 *  @param[in] length of data to send, in bytes
 *  @return 0
 */
int
corsairlink_rmi_write(
    struct libusb_device_handle* dev_handle, uint8_t endpoint, uint8_t* data, int length )
{
    int bytes_transferred;
    int rr;

    rr = libusb_interrupt_transfer(
        dev_handle, endpoint, data, length, &bytes_transferred, TIMEOUT_DEFAULT );

    return rr;
}

/*! RMi Power Supply Lowlevel Read
 *  RMi Power Supply Lowlevel Read uses a interrupt transfer
 *  @param[in] handle for the data
 *  @param[in] device endpoint for the data
 *  @param[in] data to send
 *  @param[in] length of data to send, in bytes
 *  @return 0
 */
int
corsairlink_rmi_read(
    struct libusb_device_handle* dev_handle, uint8_t endpoint, uint8_t* data, int length )
{
    int bytes_transferred;
    int rr;

    rr = libusb_interrupt_transfer(
        dev_handle, endpoint, data, length, &bytes_transferred, TIMEOUT_DEFAULT );

    return rr;
}
