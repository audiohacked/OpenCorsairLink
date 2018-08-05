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

/*! \file lowlevel/coolit.c
 *  \brief Lowlevel Routines for CoolIT USB HID based devices
 */
#include "lowlevel/coolit.h"

#include <libusb.h>
#include <stdio.h>

#define HID_SET_REPORT 0x09
#define HID_GET_REPORT 0x01
#define HID_REPORT_TYPE_INPUT 0x01
#define HID_REPORT_TYPE_OUTPUT 0x02
#define HID_REPORT_TYPE_FEATURE 0x03
#define TIMEOUT_DEFAULT 1000
#define INTERFACE_NUMBER 0
#define INTERRUPT_IN_ENDPOINT 0x81

// Values for bmRequestType in the Setup transaction's Data packet.
static const int CONTROL_REQUEST_TYPE_IN =
    LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE;
static const int CONTROL_REQUEST_TYPE_OUT =
    LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE;

/*! USB HID Init routine
 *  USB HID Init routine is empty
 *  @param[in] handle for the data
 *  @param[in] device endpoint for the data
 *  @return 0
 */
int
corsairlink_coolit_init( struct libusb_device_handle* dev_handle, uint8_t endpoint )
{
    return 0;
}

/*! USB HID De-Init routine
 *  USB HID De-Init routine is empty
 *  @param[in] handle for the data
 *  @param[in] device endpoint for the data
 *  @return 0
 */
int
corsairlink_coolit_deinit( struct libusb_device_handle* dev_handle, uint8_t endpoint )
{
    return 0;
}

/*! USB HID Lowlevel Write
 *  USB HID Lowlevel Write uses a control transfer
 *  @param[in] handle for the data
 *  @param[in] device endpoint for the data
 *  @param[in] data to send
 *  @param[in] length of data to send, in bytes
 *  @return 0
 */
int
corsairlink_coolit_write(
    struct libusb_device_handle* dev_handle, uint8_t endpoint, uint8_t* data, int length )
{
    int bytes_transferred;
    int rr;

    rr = libusb_control_transfer(
        dev_handle, CONTROL_REQUEST_TYPE_OUT, HID_SET_REPORT, /** HID Set_Report */
        ( HID_REPORT_TYPE_OUTPUT << 8 ) | 0x00, INTERFACE_NUMBER, data, length, TIMEOUT_DEFAULT );

    return rr;
}

/*! USB HID Lowlevel Read
 *  USB HID Lowlevel Write uses a control transfer
 *  @param[in] handle for the data
 *  @param[in] device endpoint for the data
 *  @param[out] data received
 *  @param[in] length of data to received, in bytes
 *  @return 0
 */
int
corsairlink_coolit_read(
    struct libusb_device_handle* dev_handle, uint8_t endpoint, uint8_t* data, int length )
{
    int bytes_transferred;
    int rr;

    rr = libusb_interrupt_transfer(
        dev_handle, endpoint, data, length, &bytes_transferred, TIMEOUT_DEFAULT );

    return rr;
}
