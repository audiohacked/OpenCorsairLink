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

#ifndef _DRIVER_H
#define _DRIVER_H

struct corsair_device_driver {
    /** device communication helper functions */
    int (*init)(struct libusb_device_handle*, uint8_t);
    int (*deinit)(struct libusb_device_handle*, uint8_t);
    int (*read)(struct libusb_device_handle*, uint8_t, uint8_t*, int);
    int (*write)(struct libusb_device_handle*, uint8_t, uint8_t*, int);

    /** protocol functions */
    int (*name)(struct corsair_device_info *dev, struct libusb_device_handle *handle, char *name, size_t name_size);
    int (*vendor)(struct corsair_device_info *dev, struct libusb_device_handle *handle, char *name, size_t name_size);
    int (*product)(struct corsair_device_info *dev, struct libusb_device_handle *handle, char *name, size_t name_size);
    int (*device_id)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t *device_id);
    int (*fw_version)(struct corsair_device_info *dev, struct libusb_device_handle *handle, char *name, size_t name_size);

    int (*led)(struct corsair_device_info *dev, struct libusb_device_handle *handle, struct color *l, struct color *w, uint8_t t, uint8_t e);
    int (*temperature)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t select, uint16_t *temp);
    int (*tempsensorscount)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t *temperature_sensors_count);

    struct fan_functions {
        int (*count)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t *fan_count);
        int (*profile)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t selector, uint8_t *profile, uint16_t *data);
        int (*custom)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t selector, struct fan_table *custom_profile);
        int (*speed)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t selector, uint16_t *speed, uint16_t *maxspeed);
        int (*print_mode)(uint8_t mode, uint16_t data, char *modestr, size_t modestr_size);
    } fan;

    struct pump_functions {
        int (*profile)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t *profile);
        int (*custom)(struct corsair_device_info *dev, struct libusb_device_handle *handle, struct fan_table *custom_profile);
        int (*speed)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint16_t *speed, uint16_t *maxspeed);
    } pump;

    struct power_functions {
        int (*supply_voltage)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint16_t *volts);
        int (*total_wattage)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint16_t *watts);
        int (*select)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t output_select);
        int (*voltage)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint16_t *volts);
        int (*amperage)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint16_t *amps);
        int (*wattage)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint16_t *watts);
    } power;

    struct time_functions {
        int (*powered)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint32_t *v32);
        int (*uptime)(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint32_t *v32);
    } psu_time;

    int (*fan_speed_read)();
};

extern struct corsair_device_driver corsairlink_driver_hid;
extern struct corsair_device_driver corsairlink_driver_asetek;
extern struct corsair_device_driver corsairlink_driver_rmi;
extern struct corsair_device_driver corsairlink_driver_dongle;

#endif
