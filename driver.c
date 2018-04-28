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

#include <libusb.h>
#include "device.h"
#include "driver.h"
#include "lowlevel/asetek.h"
#include "lowlevel/commanderpro.h"
#include "lowlevel/hid.h"
#include "lowlevel/rmi.h"
#include "protocol/asetek/core.h"
#include "protocol/asetekpro/core.h"
#include "protocol/commanderpro/core.h"
#include "protocol/hid/core.h"
#include "protocol/rmi/core.h"

struct corsair_device_driver corsairlink_driver_hid = {
    .init = corsairlink_hid_init,
    .deinit = corsairlink_hid_deinit,
    .name = corsairlink_hid_name,
    .vendor = corsairlink_hid_vendor,
    .product = corsairlink_hid_product,
    .device_id = corsairlink_hid_device_id,
    .fw_version = corsairlink_hid_firmware_id,
    .read = corsairlink_hid_read,
    .write = corsairlink_hid_write,
    .temperature = corsairlink_hid_temperature,
    .tempsensorscount = corsairlink_hid_tempsensorscount,
    .led = {
        .static_color = corsairlink_hid_change_led,
        .blink = NULL,
        .color_pulse = NULL,
        .color_shift = NULL,
        .rainbow = NULL,
        .temperature = NULL,
    },
    .fan = {
        .count = corsairlink_hid_fan_count,
        .custom = corsairlink_hid_fan_curve,
        .profile = corsairlink_hid_fan_mode,
        .speed = corsairlink_hid_fan_speed,
        .print_mode = corsairlink_hid_fan_print_mode,
    },
    .pump = {
        .profile = corsairlink_hid_pump_mode,
        .speed = corsairlink_hid_pump_speed,
    }
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
    .temperature = corsairlink_asetek_temperature,
    .tempsensorscount = corsairlink_asetek_tempsensorscount,
    .led = {
        .static_color = corsairlink_asetek_led_static_color,
        .blink = NULL,
        .color_pulse = NULL,
        .color_shift = NULL,
        .rainbow = NULL,
        .temperature = corsairlink_asetek_led_temperature,
    },
    .fan = {
        .count = corsairlink_asetek_fan_count,
        .custom = corsairlink_asetek_fan_curve,
        .profile = corsairlink_asetek_fan_mode,
        .speed = corsairlink_asetek_fan_speed,
        .print_mode = corsairlink_asetek_fan_print_mode,
    },
    .pump = {
        .profile = corsairlink_asetek_pump_mode,
        .speed = corsairlink_asetek_pump_speed,
    }
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
    // .led = corsairlink_asetek_change_led,
    .temperature = corsairlink_asetekpro_temperature,
    .tempsensorscount = corsairlink_asetekpro_tempsensorscount,
    .led = {
        .static_color = corsairlink_asetekpro_led_static_color,
        .blink = corsairlink_asetekpro_led_blink,
        .color_pulse = corsairlink_asetekpro_led_color_pulse,
        .color_shift = corsairlink_asetekpro_led_color_shift,
        .rainbow = corsairlink_asetekpro_led_rainbow,
        .temperature = corsairlink_asetekpro_led_temperature,
    },
    .fan = {
        .count = corsairlink_asetek_fan_count,
        .custom = corsairlink_asetek_fan_curve,
        .profile = corsairlink_asetek_fan_mode,
        .speed = corsairlink_asetekpro_fan_speed,
        .print_mode = corsairlink_asetek_fan_print_mode,
    },
    .pump = {
        .profile = corsairlink_asetek_pump_mode,
        .speed = corsairlink_asetekpro_pump_speed,
    }
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
    .temperature = corsairlink_commanderpro_temperature,
    .tempsensorscount = corsairlink_commanderpro_tempsensorscount,
    .led = {
        .static_color = NULL,
        .blink = NULL,
        .color_pulse = NULL,
        .color_shift = NULL,
        .rainbow = NULL,
        .temperature = NULL,
    },
    .fan = {
        // .count = corsairlink_commanderpro_get_fan_setup_mask,
        // .custom = corsairlink_commanderpro_fan_curve,
        // .profile = corsairlink_commanderpro_fan_mode,
        .speed = corsairlink_commanderpro_get_fan_speed_rpm,
        // .print_mode = corsairlink_commanderpro_fan_print_mode,
    },
    .power = {
        .voltage = corsairlink_commanderpro_voltage,
    }
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
    .temperature = corsairlink_rmi_temperature,
    .power = {
        .supply_voltage = corsairlink_rmi_power_supply_voltage,
        .total_wattage = corsairlink_rmi_power_total_wattage,
        .sensor_select = corsairlink_rmi_sensor_select,
        .voltage = corsairlink_rmi_output_volts,
        .amperage = corsairlink_rmi_output_amps,
        .wattage = corsairlink_rmi_output_watts,
    },
    .psu_time = {
        .powered = corsairlink_rmi_time_powered,
        .uptime = corsairlink_rmi_time_uptime,
    }
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
    .write = corsairlink_rmi_write
};

