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
#include "protocol/asetekpro.h"

struct corsair_lowlevel_driver corsairlink_lowlevel_asetekpro = {
    .init = corsairlink_asetek_init,
    .deinit = corsairlink_asetek_deinit,
    .read = corsairlink_asetek_read,
    .write = corsairlink_asetek_write,
};

struct corsair_device_driver corsairlink_driver_asetekpro = {
    .name = corsairlink_asetek_name,
    .vendor = corsairlink_asetek_vendor,
    .product = corsairlink_asetek_product,
    .device_id = corsairlink_asetek_device_id,
    .fw_version = corsairlink_asetekpro_firmware_id,
    .temperature =
        {
            .read = corsairlink_asetekpro_temperature,
            .count = corsairlink_asetekpro_tempsensorscount,
        },
    .led =
        {
            .static_color = corsairlink_asetekpro_led_static_color,
            .blink = corsairlink_asetekpro_led_blink,
            .color_pulse = corsairlink_asetekpro_led_color_pulse,
            .color_shift = corsairlink_asetekpro_led_color_shift,
            .rainbow = corsairlink_asetekpro_led_rainbow,
            .temperature = corsairlink_asetekpro_led_temperature,
        },
    .fan =
        {
            .count = corsairlink_asetekpro_fan_count,
            .speed = corsairlink_asetekpro_fan_speed,
            .print_mode = corsairlink_asetekpro_fan_print_mode,
            .profile =
                {
                    .read_profile = corsairlink_asetekpro_fan_mode_read,
                    .read_rpm = corsairlink_unspported_fan,
                    .read_pwm = corsairlink_unspported_fan,
                    .write_profile_custom = corsairlink_unspported_fan,
                    .write_profile_default = corsairlink_unspported_fan,
                    .write_profile_performance = corsairlink_asetekpro_fan_mode_performance,
                    .write_profile_balanced = corsairlink_asetekpro_fan_mode_balanced,
                    .write_profile_quiet = corsairlink_asetekpro_fan_mode_quiet,
                    .write_rpm = corsairlink_asetekpro_fan_mode_rpm,
                    .write_pwm = corsairlink_asetekpro_fan_mode_pwm,
                    .write_custom_curve = corsairlink_asetekpro_fan_curve,
                },
        },
    .pump =
        {
            .profile =
                {
                    .read_profile = corsairlink_asetekpro_pump_mode_read,
                    .write_profile_default = corsairlink_unspported_pump,
                    .write_profile_performance = corsairlink_asetekpro_pump_mode_performance,
                    .write_profile_balanced = corsairlink_asetekpro_pump_mode_balanced,
                    .write_profile_quiet = corsairlink_asetekpro_pump_mode_quiet,
                    .write_profile_custom = corsairlink_unspported_pump,
                    .write_custom_curve = corsairlink_unspported_pump,
                },
            // .profile = corsairlink_asetek_pump_mode,
            .speed = corsairlink_asetekpro_pump_speed,
        },
};
