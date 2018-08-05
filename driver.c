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

#include "driver.h"

#include "device.h"
#include "lowlevel/asetek.h"
#include "lowlevel/commanderpro.h"
#include "lowlevel/coolit.h"
#include "lowlevel/rmi.h"
#include "print.h"
#include "protocol/asetek.h"
#include "protocol/asetekpro.h"
#include "protocol/commanderpro.h"
#include "protocol/coolit.h"
#include "protocol/rmi.h"

int
corsairlink_unsupported_led(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct led_control* ctrl )
{
    msg_info( "Unsupported Generic LED Function\n" );
    return 0;
}

int
corsairlink_unspported_fan(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, struct fan_control* ctrl )
{
    msg_info( "Unsupported Generic Fan Function\n" );
    return 0;
}

int
corsairlink_unspported_pump(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    msg_info( "Unsupported Generic Pump Function\n" );
    return 0;
}

struct corsair_device_driver corsairlink_driver_coolit = {
    .init = corsairlink_coolit_init,
    .deinit = corsairlink_coolit_deinit,
    .name = corsairlink_coolit_name,
    .vendor = corsairlink_coolit_vendor,
    .product = corsairlink_coolit_product,
    .device_id = corsairlink_coolit_device_id,
    .fw_version = corsairlink_coolit_firmware_id,
    .read = corsairlink_coolit_read,
    .write = corsairlink_coolit_write,
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

struct corsair_device_driver corsairlink_driver_coolit_old = {
    .init = corsairlink_coolit_init,
    .deinit = corsairlink_coolit_deinit,
    .name = corsairlink_coolit_name,
    .vendor = corsairlink_coolit_vendor,
    .product = corsairlink_coolit_product,
    .device_id = corsairlink_coolit_device_id,
    .fw_version = corsairlink_coolit_firmware_id,
    .read = corsairlink_coolit_read,
    .write = corsairlink_coolit_write,
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

struct corsair_device_driver corsairlink_driver_asetekpro = {
    .init = corsairlink_asetek_init,
    .deinit = corsairlink_asetek_deinit,
    .name = corsairlink_asetek_name,
    .vendor = corsairlink_asetek_vendor,
    .product = corsairlink_asetek_product,
    .device_id = corsairlink_asetek_device_id,
    .fw_version = corsairlink_asetekpro_firmware_id,
    .read = corsairlink_asetek_read,
    .write = corsairlink_asetek_write,
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
            .count = corsairlink_asetek_fan_count,
            .speed = corsairlink_asetekpro_fan_speed,
            .print_mode = corsairlink_asetek_fan_print_mode,
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
            .speed = corsairlink_asetekpro_pump_speed,
        },
};

struct corsair_device_driver corsairlink_driver_commanderpro = {
    .init = corsairlink_commanderpro_init,
    .deinit = corsairlink_commanderpro_deinit,
    .name = corsairlink_commanderpro_name,
    .vendor = corsairlink_commanderpro_vendor,
    .product = corsairlink_commanderpro_product,
    .device_id = corsairlink_commanderpro_device_id,
    .fw_version = corsairlink_commanderpro_firmware_id,
    .read = corsairlink_commanderpro_read,
    .write = corsairlink_commanderpro_write,
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
            .count = corsairlink_unspported_fan,
            .speed = corsairlink_unspported_fan,
            .print_mode = corsairlink_commanderpro_fan_print_mode,
            .profile =
                {
                    .read_profile = corsairlink_unspported_fan,
                    .read_rpm = corsairlink_commanderpro_get_fan_speed_rpm,
                    .read_pwm = corsairlink_commanderpro_get_fan_speed_pwm,
                    .write_profile_custom = corsairlink_unspported_fan,
                    .write_profile_default = corsairlink_unspported_fan,
                    .write_profile_performance = corsairlink_unspported_fan,
                    .write_profile_balanced = corsairlink_unspported_fan,
                    .write_profile_quiet = corsairlink_unspported_fan,
                    .write_rpm = corsairlink_unspported_fan,
                    .write_pwm = corsairlink_unspported_fan,
                    .write_custom_curve = corsairlink_unspported_fan,
                },
        },
    .power =
        {
            .voltage = corsairlink_commanderpro_voltage,
        },
};

struct corsair_device_driver corsairlink_driver_rmi = {
    .init = corsairlink_rmi_init,
    .deinit = corsairlink_rmi_deinit,
    .name = corsairlink_rmi_name,
    .vendor = corsairlink_rmi_vendor,
    .product = corsairlink_rmi_product,
    .device_id = corsairlink_rmi_device_id,
    .fw_version = corsairlink_rmi_firmware_id,
    .read = corsairlink_rmi_read,
    .write = corsairlink_rmi_write,
    .temperature =
        {
            .read = corsairlink_rmi_temperature,
        },
    .power =
        {
            .supply_voltage = corsairlink_rmi_power_supply_voltage,
            .total_wattage = corsairlink_rmi_power_total_wattage,
            .sensor_select = corsairlink_rmi_sensor_select,
            .voltage = corsairlink_rmi_output_volts,
            .amperage = corsairlink_rmi_output_amps,
            .wattage = corsairlink_rmi_output_watts,
        },
    .psu_time =
        {
            .powered = corsairlink_rmi_time_powered,
            .uptime = corsairlink_rmi_time_uptime,
        },
};

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
