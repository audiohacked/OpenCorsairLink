/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017-2019  Sean Nelson <audiohacked@gmail.com>

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

#include "driver.h"

#include "device.h"
#include "lowlevel/rmi.h"
#include "print.h"
#include "protocol/rmi.h"

struct corsair_device_driver corsairlink_driver_dongle = {
    .init = corsairlink_rmi_init,
    .deinit = corsairlink_rmi_deinit,
    .name = corsairlink_rmi_name,
    .vendor = corsairlink_rmi_vendor,
    .product = corsairlink_rmi_product,
    .device_id = corsairlink_rmi_device_id,
    .fw_version = corsairlink_rmi_firmware_id,
    .read = corsairlink_rmi_read,
    .write = corsairlink_rmi_write,
};
