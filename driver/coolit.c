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
#include "lowlevel/coolit.h"
#include "print.h"
#include "protocol/coolit.h"

struct corsair_lowlevel_driver corsairlink_lowlevel_coolit = {
    .init = corsairlink_coolit_init,
    .deinit = corsairlink_coolit_deinit,
    .read = corsairlink_coolit_read,
    .write = corsairlink_coolit_write,
};

struct corsair_device_driver corsairlink_driver_coolit = {
    .name = corsairlink_coolit_name,
    .vendor = corsairlink_coolit_vendor,
    .product = corsairlink_coolit_product,
    .device_id = corsairlink_coolit_device_id,
    .fw_version = corsairlink_coolit_firmware_id,
    .temperature =
        {
            .read = corsairlink_coolit_temperature,
            .count = corsairlink_coolit_tempsensorscount,
        },
    .led =
        {
            .static_color = corsairlink_coolit_change_led,
            .blink = corsairlink_unsupported_led,
            .color_pulse = corsairlink_unsupported_led,
            .color_shift = corsairlink_unsupported_led,
            .rainbow = corsairlink_unsupported_led,
            .temperature = corsairlink_unsupported_led,
        },
    .fan =
        {
            .count = corsairlink_coolit_fan_count,
            .profile =
                {
                    .read_profile = corsairlink_coolit_fan_mode_read,
                    .read_rpm = corsairlink_coolit_fan_mode_read_rpm,
                    .read_pwm = corsairlink_coolit_fan_mode_read_pwm,
                    .write_profile_custom = corsairlink_coolit_fan_mode_custom,
                    .write_profile_default = corsairlink_coolit_fan_mode_default,
                    .write_profile_performance = corsairlink_coolit_fan_mode_performance,
                    .write_profile_balanced = corsairlink_coolit_fan_mode_balanced,
                    .write_profile_quiet = corsairlink_coolit_fan_mode_quiet,
                    .write_rpm = corsairlink_coolit_fan_mode_rpm,
                    .write_pwm = corsairlink_coolit_fan_mode_pwm,
                    .write_custom_curve = corsairlink_coolit_fan_curve,
                },
            .speed = corsairlink_coolit_fan_speed,
            .print_mode = corsairlink_coolit_fan_print_mode,
        },
    .pump =
        {
            .profile =
                {
                    .read_profile = corsairlink_coolit_pump_mode_read,
                    .write_profile_default = corsairlink_coolit_pump_mode_default,
                    .write_profile_performance = corsairlink_coolit_pump_mode_performance,
                    .write_profile_balanced = corsairlink_coolit_pump_mode_balanced,
                    .write_profile_quiet = corsairlink_coolit_pump_mode_quiet,
                    .write_profile_custom = corsairlink_coolit_pump_mode_custom,
                    .write_custom_curve = corsairlink_coolit_pump_curve,
                },
            // .profile = corsairlink_coolit_pump_mode,
            .speed = corsairlink_coolit_pump_speed,
        },
};
