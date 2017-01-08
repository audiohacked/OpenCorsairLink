/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017  Sean Nelson <audiohacked@gmail.com>

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

#include <libusb.h>
#include "device.h"
#include "lowlevel/asetek4.h"
#include "lowlevel/hid.h"
#include "protocol/asetek4/core.h"
#include "protocol/hid/core.h"

struct corsair_device_driver corsairlink_driver_hid = {
	.init = corsairlink_hid_init,
	.deinit = corsairlink_hid_deinit,
	.device_id = corsairlink_hid_device_id,
	.read = corsairlink_hid_read,
	.write = corsairlink_hid_write,
	.led = corsairlink_hid_change_led,
};

struct corsair_device_driver corsairlink_driver_asetek = {
	.init = corsairlink_asetek_init,
	.deinit = corsairlink_asetek_deinit,
	.device_id = corsairlink_asetek_device_id,
	.read = corsairlink_asetek_read,
	.write = corsairlink_asetek_write,
	.led = corsairlink_asetek_change_led,
	.fan_control = {
		.custom = corsairlink_asetek_fan_curve,
	}
};

struct corsair_device_info corsairlink_devices[] = {
	{
		.vendor_id = 0x1b1c,
		.product_id = 0x0c04,
		.device_id = 0x3b, /** H80i */
		.name = "H80i",
		.read_endpoint = 0x01|LIBUSB_ENDPOINT_IN,
		.write_endpoint = LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_OUT,
		.handle = NULL,
		.context = NULL,
		.driver = &corsairlink_driver_hid,
		.led_control_count = 1,
		.fan_control_count = 4,
		.pump_index = 5,
	},
	{
		.vendor_id = 0x1b1c,
		.product_id = 0x0c04,
		.device_id = 0x3c, /** H100i */
		.name = "H100i",
		.read_endpoint = 0x01|LIBUSB_ENDPOINT_IN,
		.write_endpoint = LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_OUT,
		.handle = NULL,
		.context = NULL,
		.driver = &corsairlink_driver_hid,
		.led_control_count = 1,
		.fan_control_count = 4,
		.pump_index = 5,
	},
	{
		.vendor_id = 0x1b1c,
		.product_id = 0x0c04,
		.device_id = 0x41, /** H110i */
		.name = "H110i",
		.read_endpoint = 0x01|LIBUSB_ENDPOINT_IN,
		.write_endpoint = LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_OUT,
		.handle = NULL,
		.context = NULL,
		.driver = &corsairlink_driver_hid,
		.led_control_count = 1,
		.fan_control_count = 4,
		.pump_index = 5,
	},
	{
		.vendor_id = 0x1b1c,
		.product_id = 0x0c04,
		.device_id = 0x42, /** H110i Extreme*/
		.name = "H110i Extreme",
		.read_endpoint = 0x01|LIBUSB_ENDPOINT_IN,
		.write_endpoint = LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_OUT,
		.handle = NULL,
		.context = NULL,
		.driver = &corsairlink_driver_hid,
		.led_control_count = 1,
		.fan_control_count = 4,
		.pump_index = 5,
	},
	{
		.vendor_id = 0x1b1c,
		.product_id = 0x0c09,
		.device_id = 0x00,
		.name = "H100i V2", /** also called H100i GTX */
		.read_endpoint = 0x02|LIBUSB_ENDPOINT_IN,
		.write_endpoint = 0x02|LIBUSB_ENDPOINT_OUT,
		.handle = NULL,
		.context = NULL,
		.driver = &corsairlink_driver_asetek,
		.led_control_count = 1,
		.fan_control_count = 1,
		.pump_index = 0,
	},
	{
		.vendor_id = 0x1b1c,
		.product_id = 0x0c0a,
		.device_id = 0x00,
		.name = "H115i", /** H115i */
		.read_endpoint = 0x02|LIBUSB_ENDPOINT_IN,
		.write_endpoint = 0x02|LIBUSB_ENDPOINT_OUT,
		.handle = NULL,
		.context = NULL,
		.driver = &corsairlink_driver_asetek,
		.led_control_count = 1,
		.fan_control_count = 1,
		.pump_index = 0,
	},
};

