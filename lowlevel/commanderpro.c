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

#include <stdio.h>
#include <libusb.h>
#include "lowlevel/commanderpro.h"

#define HID_SET_REPORT 0x09 /*!< HID_SET_REPORT */
#define HID_GET_REPORT 0x01 /*!< HID_GET_REPORT */
#define HID_REPORT_TYPE_INPUT 0x01 /*!< HID_REPORT_TYPE_INPUT */
#define HID_REPORT_TYPE_OUTPUT 0x02 /*!< HID_REPORT_TYPE_OUTPUT */
#define HID_REPORT_TYPE_FEATURE 0x03 /*!< HID_REPORT_TYPE_FEATURE */
#define TIMEOUT_DEFAULT 1000 /*!< TIMEOUT_DEFAULT */
#define INTERFACE_NUMBER 0 /*!< INTERFACE_NUMBER */

int corsairlink_commanderpro_init(struct libusb_device_handle *dev_handle,
			uint8_t endpoint)
{
	int rr;

	rr = libusb_control_transfer(dev_handle, 0x40, 0x00, 0xffff, 0x0000, NULL, 0, 0);
	rr = libusb_control_transfer(dev_handle, 0x40, 0x02, 0x0002, 0x0000, NULL, 0, 0);

	return rr;
}

int corsairlink_commanderpro_deinit(struct libusb_device_handle *dev_handle,
			uint8_t endpoint)
{
	int rr;

	rr = libusb_control_transfer(dev_handle, 0x40, 0x02, 0x0004, 0x0000, NULL, 0, 200);

	return rr;
}

int corsairlink_commanderpro_write(struct libusb_device_handle *dev_handle,
			uint8_t endpoint,
			uint8_t *data,
			int length)
{
	int bytes_transferred;
	int rr;
	
	rr = libusb_bulk_transfer(dev_handle, endpoint, data, length, &bytes_transferred, TIMEOUT_DEFAULT);

	return rr;
}

int corsairlink_commanderpro_read(struct libusb_device_handle *dev_handle,
			uint8_t endpoint,
			uint8_t *data,
			int length)
{
	int bytes_transferred;
	int rr;
	
	rr = libusb_bulk_transfer(dev_handle, endpoint, data, length, &bytes_transferred, TIMEOUT_DEFAULT);

	return rr;
}
