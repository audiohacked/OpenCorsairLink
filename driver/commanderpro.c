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
#include "lowlevel/commanderpro.h"
#include "print.h"
#include "protocol/commanderpro.h"

struct corsair_lowlevel_driver corsairlink_lowlevel_commanderpro = {
    .init = corsairlink_commanderpro_init,
    .deinit = corsairlink_commanderpro_deinit,
    .read = corsairlink_commanderpro_read,
    .write = corsairlink_commanderpro_write,
};

struct corsair_device_driver corsairlink_driver_commanderpro = {
    .name = corsairlink_commanderpro_name,
    .vendor = corsairlink_commanderpro_vendor,
    .product = corsairlink_commanderpro_product,
    .device_id = corsairlink_commanderpro_device_id,
    .fw_version = corsairlink_commanderpro_firmware_id,
    .temperature =
        {
            .read = corsairlink_commanderpro_temperature,
            .count = corsairlink_commanderpro_tempsensorscount,
        },
    .led =
        {
            .static_color = corsairlink_unsupported_led,
            .blink = corsairlink_unsupported_led,
            .color_pulse = corsairlink_unsupported_led,
            .color_shift = corsairlink_unsupported_led,
            .rainbow = corsairlink_unsupported_led,
            .temperature = corsairlink_unsupported_led,
        },
    .fan =
        {
            .count = corsairlink_commanderpro_fan_count,
            .speed = corsairlink_unspported_fan,
            .print_mode = corsairlink_commanderpro_fan_print_mode,
            .profile =
                {
                    .read_profile = corsairlink_commanderpro_fan_mode_read,
                    .read_rpm = corsairlink_commanderpro_get_fan_speed_rpm,
                    .read_pwm = corsairlink_commanderpro_get_fan_speed_pwm,
                    .write_profile_custom = corsairlink_unspported_fan,
                    .write_profile_default = corsairlink_unspported_fan,
                    .write_profile_performance = corsairlink_unspported_fan,
                    .write_profile_balanced = corsairlink_unspported_fan,
                    .write_profile_quiet = corsairlink_unspported_fan,
                    .write_rpm = corsairlink_commanderpro_set_fan_speed_rpm,
                    .write_pwm = corsairlink_commanderpro_set_fan_speed_pwm,
                    .write_custom_curve = corsairlink_unspported_fan,
                },
        },
    .power =
        {
            .voltage = corsairlink_commanderpro_voltage,
        },
};
