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

#ifndef _DRIVER_H
#define _DRIVER_H

#include "common.h"

struct corsair_device_driver {
    /** device communication helper functions */
    int (*init)  (struct libusb_device_handle*, uint8_t);
    int (*deinit)(struct libusb_device_handle*, uint8_t);
    int (*read)  (struct libusb_device_handle*, uint8_t, uint8_t*, int);
    int (*write) (struct libusb_device_handle*, uint8_t, uint8_t*, int);

    /** protocol functions */
    int (*name)      (struct corsair_device_info *dev, struct libusb_device_handle *handle, char *name, uint8_t name_size);
    int (*vendor)    (struct corsair_device_info *dev, struct libusb_device_handle *handle, char *name, uint8_t name_size);
    int (*product)   (struct corsair_device_info *dev, struct libusb_device_handle *handle, char *name, uint8_t name_size);
    int (*device_id) (struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t *device_id);
    int (*fw_version)(struct corsair_device_info *dev, struct libusb_device_handle *handle, char *name, uint8_t name_size);

    int (*temperature)     (struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t sensor_select, char *temp, uint8_t str_len);
    int (*tempsensorscount)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t *temperature_sensors_count);

    struct led_functions {
        int (*static_color)(struct corsair_device_info *dev, struct libusb_device_handle *handle, struct color *l, struct color *w, uint8_t t, uint8_t e);
        int (*blink)       (struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t speed, uint8_t led_count, struct color *led1, struct color *led2, struct color *led3, struct color *led4, struct color *led5, struct color *led6, struct color *led7);
        int (*color_pulse) (struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t speed, uint8_t led_count, struct color *led1, struct color *led2, struct color *led3, struct color *led4, struct color *led5, struct color *led6, struct color *led7);
        int (*color_shift) (struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t speed, uint8_t led_count, struct color *led1, struct color *led2, struct color *led3, struct color *led4, struct color *led5, struct color *led6, struct color *led7);
        int (*rainbow)     (struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t speed);
        int (*temperature) (struct corsair_device_info *dev, struct libusb_device_handle *handle, struct led_temperatures *led_temps, struct color *temp1_led, struct color *temp2_led, struct color *temp3_led);
    } led;

    struct fan_functions {
        int (*count)  (struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t *fan_count);
        int (*profile)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t selector, uint8_t *profile, uint16_t *data);
        int (*custom) (struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t selector, struct fan_table *custom_profile);
        int (*speed)  (struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t selector, uint16_t *speed, uint16_t *maxspeed);
        int (*print_mode)(uint8_t mode, uint16_t data, char *modestr, uint8_t modestr_size);
    } fan;

    struct pump_functions {
        int (*profile)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t *profile);
        int (*custom) (struct corsair_device_info *dev, struct libusb_device_handle *handle, struct fan_table *custom_profile);
        int (*speed)  (struct corsair_device_info *dev, struct libusb_device_handle *handle, uint16_t *speed, uint16_t *maxspeed);
    } pump;

    struct power_functions {
        int (*supply_voltage)(struct corsair_device_info *dev, struct libusb_device_handle *handle, char *volts, uint8_t volts_str_len);
        int (*total_wattage) (struct corsair_device_info *dev, struct libusb_device_handle *handle, char *watts, uint8_t watts_str_len);
        int (*sensor_select) (struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t sensor_select);
        int (*voltage)       (struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t sensor_select, char *volts, uint8_t volts_str_len);
        int (*amperage)      (struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t sensor_select, char *amps, uint8_t amps_str_len);
        int (*wattage)       (struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t sensor_select, char *watts, uint8_t watts_str_len);
    } power;

    struct time_functions {
        int (*powered)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint32_t *v32);
        int (*uptime) (struct corsair_device_info *dev, struct libusb_device_handle *handle, uint32_t *v32);
    } psu_time;

    int (*fan_speed_read)();
};

extern struct corsair_device_driver corsairlink_driver_asetek;
extern struct corsair_device_driver corsairlink_driver_asetekpro;
extern struct corsair_device_driver corsairlink_driver_commanderpro;
extern struct corsair_device_driver corsairlink_driver_dongle;
extern struct corsair_device_driver corsairlink_driver_hid;
extern struct corsair_device_driver corsairlink_driver_rmi;

#endif
