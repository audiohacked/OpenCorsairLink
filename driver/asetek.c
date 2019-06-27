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
#include "lowlevel/asetek.h"
#include "print.h"
#include "protocol/asetek.h"

struct corsair_device_driver corsairlink_driver_asetek = {
    .init = corsairlink_asetek_init,
    .deinit = corsairlink_asetek_deinit,
    .name = corsairlink_asetek_name,
    .vendor = corsairlink_asetek_vendor,
    .product = corsairlink_asetek_product,
    .device_id = corsairlink_asetek_device_id,
    .fw_version = corsairlink_asetek_firmware_id,
    .read = corsairlink_asetek_read,
    .write = corsairlink_asetek_write,
    .temperature =
        {
            .read = corsairlink_asetek_temperature,
            .count = corsairlink_asetek_tempsensorscount,
        },
    .led =
        {
            .static_color = corsairlink_asetek_led_static_color,
            .blink = corsairlink_unsupported_led,
            .color_pulse = corsairlink_unsupported_led,
            .color_shift = corsairlink_unsupported_led,
            .rainbow = corsairlink_unsupported_led,
            .temperature = corsairlink_asetek_led_temperature,
        },
    .fan =
        {
            .count = corsairlink_asetek_fan_count,
            .profile =
                {
                    .read_profile = corsairlink_asetek_fan_mode_read,
                    .read_rpm = corsairlink_unspported_fan,
                    .read_pwm = corsairlink_unspported_fan,
                    .write_profile_custom = corsairlink_unspported_fan,
                    .write_profile_default = corsairlink_unspported_fan,
                    .write_profile_performance = corsairlink_asetek_fan_mode_performance,
                    .write_profile_balanced = corsairlink_asetek_fan_mode_balanced,
                    .write_profile_quiet = corsairlink_asetek_fan_mode_quiet,
                    .write_rpm = corsairlink_unspported_fan,
                    .write_pwm = corsairlink_unspported_fan,
                    .write_custom_curve = corsairlink_asetek_fan_curve,
                },
            .speed = corsairlink_asetek_fan_speed,
            .print_mode = corsairlink_asetek_fan_print_mode,
        },
    .pump =
        {
            .profile =
                {
                    .read_profile = corsairlink_unspported_pump,
                    .write_profile_default = corsairlink_unspported_pump,
                    .write_profile_performance = corsairlink_unspported_pump,
                    .write_profile_balanced = corsairlink_unspported_pump,
                    .write_profile_quiet = corsairlink_unspported_pump,
                    .write_profile_custom = corsairlink_unspported_pump,
                    .write_custom_curve = corsairlink_unspported_pump,
                },
            // .profile = corsairlink_asetek_pump_mode,
            .speed = corsairlink_asetek_pump_speed,
        },
};
