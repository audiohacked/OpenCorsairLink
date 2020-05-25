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

#ifndef _UNSUPPORTED_H
#define _UNSUPPORTED_H

int
corsairlink_unsupported_led(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct led_control* ctrl );

int
corsairlink_unsupported_fan(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct fan_control* ctrl );

int
corsairlink_unsupported_pump(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl );

#endif // _UNSUPPORTED_H
