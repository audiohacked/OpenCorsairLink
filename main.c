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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libusb.h>
#include "options.h"
#include "device.h"
#include "driver.h"
#include "print.h"
#include "scan.h"
#include "lowlevel/asetek4.h"
#include "lowlevel/hid.h"
#include "lowlevel/rmi.h"
#include "protocol/hid/core.h"
#include "protocol/rmi/core.h"

int psu_settings(struct corsair_device_scan scanned_device, struct option_parse_return settings) {
    int rr;
    int ii;
    char name[32];
    name[sizeof(name) - 1] = 0;
    uint32_t time = 0;
    uint16_t supply_volts, supply_watts, temperature,
        output_volts, output_amps, output_watts;
    struct corsair_device_info *dev;
    struct libusb_device_handle *handle;

    dev = scanned_device.device;
    handle = scanned_device.handle;
    msg_debug("DEBUG: shortcuts set\n");

    rr = dev->driver->init(handle, dev->write_endpoint);
    msg_debug("DEBUG: init done\n");

    /* fetch device name, vendor name, product name */
    rr = dev->driver->name(dev, handle, name, sizeof(name));
    rr = dev->driver->vendor(dev, handle, name, sizeof(name));
    msg_info("Vendor: %s\n", name);
    rr = dev->driver->product(dev, handle, name, sizeof(name));
    msg_info("Product: %s\n", name);
    rr = dev->driver->fw_version(dev, handle, name, sizeof(name));
    msg_info("Firmware: %s\n", name);
    msg_debug("DEBUG: string done\n");

    /* fetch temperatures */
    for (ii=0; ii<2; ii++) {
        rr = dev->driver->temperature(dev, handle, ii, &temperature);
        msg_info("Temperature %d: %5.2f C\n", ii, convert_bytes_double(temperature));
    }

    /* fetch device powered time and device uptime */
    rr = dev->driver->psu_time.powered(dev, handle, &time);
    msg_info("Powered: %u (%dd.  %dh)\n",
        time, time/(24*60*60), time/(60*60)%24);
    rr = dev->driver->psu_time.uptime(dev, handle, &time);
    msg_info("Uptime: %u (%dd.  %dh)\n",
        time, time/(24*60*60), time/(60*60)%24);
    msg_debug("DEBUG: time done\n");

    /* fetch Supply Voltage and Total Watts Consumming */
    rr = dev->driver->power.supply_voltage(dev, handle, &supply_volts);
    msg_info("Supply Voltage: %5.2f\n", convert_bytes_double(supply_volts));
    rr = dev->driver->power.total_wattage(dev, handle, &supply_watts);
    msg_info("Total Watts: %5.2f\n", convert_bytes_double(supply_watts));
    msg_debug("DEBUG: supply done\n");

    /* fetch PSU output */
    for (ii=0; ii<3; ii++) {
        if (ii==0)
            msg_info("Output 12v:\n");
        if (ii==1)
            msg_info("Output 5v:\n");
        if (ii==2)
            msg_info("Output 3.3v:\n");

        rr = dev->driver->power.select(dev, handle, ii);
        rr = dev->driver->power.voltage(dev, handle, &output_volts);
        msg_info("\tVoltage %5.2f\n", convert_bytes_double(output_volts));

        rr = dev->driver->power.amperage(dev, handle, &output_amps);
        msg_info("\tAmps %5.2f\n", convert_bytes_double(output_amps));

        rr = dev->driver->power.wattage(dev, handle, &output_watts);
        msg_info("\tWatts %5.2f\n", convert_bytes_double(output_watts));
    }
    rr = dev->driver->power.select(dev, handle, 0);

    rr = dev->driver->deinit(handle, dev->write_endpoint);

    return 0;
}

int hydro_settings(struct corsair_device_scan scanned_device, struct option_parse_return settings) {
    int rr;
    int ii;
    char name[20];
    memset(name, '\0', sizeof(name));
    char fan_mode_string[64];
    memset(fan_mode_string, '\0', sizeof(fan_mode_string));
    struct corsair_device_info *dev;
    struct libusb_device_handle *handle;
    uint16_t temperature;
    uint8_t temperature_sensors_count=0;
    uint8_t fan_count=0;
    double celsius = 0;
    uint8_t pump_mode = 0;
    uint16_t pump_speed = 0;
    uint16_t pump_max_speed = 0;
    uint8_t fan_index = 0;
    uint8_t fan_mode = UNDEFINED;
    uint16_t fan_data = 0;
    uint16_t fan_speed = 0;
    uint16_t fan_max_speed = 0;

    dev = scanned_device.device;
    handle = scanned_device.handle;
    msg_debug("DEBUG: shortcuts set\n");

    rr = dev->driver->init(handle, dev->write_endpoint);
    msg_debug("DEBUG: init done\n");

    /* fetch device name, vendor name, product name */
    rr = dev->driver->vendor(dev, handle, name, sizeof(name));
    msg_info("Vendor: %s\n", name);
    rr = dev->driver->product(dev, handle, name, sizeof(name));
    msg_info("Product: %s\n", name);
    rr = dev->driver->fw_version(dev, handle, name, sizeof(name));
    msg_info("Firmware: %s\n", name);

    /* get number of temperature sensors */
    rr = dev->driver->tempsensorscount(dev, handle, &temperature_sensors_count);

    for (ii=0; ii<temperature_sensors_count; ii++) {
        rr = dev->driver->temperature(dev, handle, ii, &temperature);
        if (dev->driver == &corsairlink_driver_asetek) {
            uint8_t v1 = (temperature>>8);
            uint8_t v2 = (temperature&0xFF);
            msg_debug("DEBUG: %02X %02X\n", v1, v2);
            celsius = (double)v1 + ((double)v2/10);
        } else if (dev->driver == &corsairlink_driver_hid) {
            celsius = (double)temperature / 256;
        }
        msg_info("Temperature %d: %5.2f\n", ii, (double)celsius);
        if (dev->driver == &corsairlink_driver_asetek) {
            break;
        }
    }

    /* get number of fans */
    rr = dev->driver->fan.count(dev, handle, &fan_count);

    for (ii=0; ii<fan_count; ii++) {
        fan_mode = UNDEFINED;
        fan_speed = 0;
        fan_max_speed = 0;
        fan_data = 0;
        rr = dev->driver->fan.profile(dev, handle, ii, &fan_mode, &fan_data);
        rr = dev->driver->fan.print_mode(fan_mode, fan_data, fan_mode_string, sizeof(fan_mode_string));
        rr = dev->driver->fan.speed(dev, handle, ii, &fan_speed, &fan_max_speed);
        msg_info("Fan %d:\t%s\n", ii, fan_mode_string);
        msg_info("\tCurrent/Max Speed %i/%i RPM\n", fan_speed, fan_max_speed);
    }

    rr = dev->driver->pump.profile(dev, handle, &pump_mode);
    rr = dev->driver->pump.speed(dev, handle, &pump_speed, &pump_max_speed);

    msg_info("Pump:\tMode 0x%02X\n", pump_mode);
    msg_info("\tCurrent/Max Speed %i/%i RPM\n", pump_speed, pump_max_speed);

    rr = dev->driver->led(dev, handle, &settings.led_color, &settings.warning_led, settings.warning_led_temp, (settings.warning_led_temp > -1));

    if (dev->driver == &corsairlink_driver_asetek) {
        if (settings.fan1.s6 != 0)
            dev->driver->fan.custom(dev, handle, 0, &settings.fan1);
        if (settings.pump_mode != DEFAULT)
            dev->driver->pump.profile(dev, handle, &settings.pump_mode);
    } else
    if (dev->driver == &corsairlink_driver_hid) {
        if (settings.pump_mode != DEFAULT) {
            msg_info("Setting pump to mode: %i\n", settings.pump_mode);
            rr = dev->driver->pump.profile(dev, handle, &settings.pump_mode);
            pump_mode = 0;
            pump_speed = 0;
            pump_max_speed = 0;
            rr = dev->driver->pump.profile(dev, handle, &pump_mode);
            // sleep 3 seconds for pump to reach new value
            sleep(3);
            rr = dev->driver->pump.speed(dev, handle, &pump_speed, &pump_max_speed);
            msg_info("Pump:\tMode 0x%02X\n", pump_mode);
            msg_info("\tCurrent/Max Speed %i/%i RPM\n", pump_speed, pump_max_speed);
        }
        if (settings.fan > 0 && settings.fan < fan_count + 1) {

            fan_mode = settings.fan_mode;
            fan_data = settings.fan_data;
            rr = dev->driver->fan.profile(dev, handle, settings.fan - 1, &fan_mode, &fan_data);
        }
    }

    rr = dev->driver->deinit(handle, dev->write_endpoint);
    msg_debug("DEBUG: deinit done\n");

    return 0;
}

int main(int argc, char *argv[])
{
    int rr; // result from libusb functions

    int8_t device_number = -1;

    int scanlist_count = 0;

    struct option_flags flags;
    struct option_parse_return settings;

    options_parse(argc, argv, &flags, &device_number,
        &settings);

    libusb_context *context = NULL;

    rr = libusb_init(&context);
    if (rr < 0) {
        msg_info("Init Error %d\n", rr);
        return 1;
    }
    libusb_set_debug(context, 3);

    corsairlink_device_scanner(context, &scanlist_count);
    msg_debug("DEBUG: scan done, start routines\n");
    msg_debug("DEBUG: device_number = %d\n", device_number);

    if (device_number >= 0) {
        if(device_number >= scanlist_count) {
            msg_info("Detected %d device(s), submitted device %d is out of range\n", scanlist_count, device_number);
        } else {
            if (scanlist[device_number].device->driver == &corsairlink_driver_rmi) {
                psu_settings(scanlist[device_number], settings);
            } else {
                hydro_settings(scanlist[device_number], settings);
            }
        }
    }

exit:
    corsairlink_close(context);
    return 0;
}
