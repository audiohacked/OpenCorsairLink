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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libusb.h>
#include "device.h"
#include "driver.h"
#include "print.h"
#include "logic/scan.h"
#include "logic/options.h"

int commanderpro_settings(struct corsair_device_scan scanned_device,
            struct option_flags flags,
            struct option_parse_return settings)
{
    int rr;
    int ii;
    char name[32];
    name[sizeof(name) - 1] = 0;
    double output_volts;
    uint32_t time = 0;
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
    for (ii=0; ii<4; ii++) {
        // char temperature[16];
        double temperature;
        rr = dev->driver->temperature(dev, handle, ii, &temperature);
        msg_info("Temperature %d: %5.2f C\n", ii, temperature);
    }

    /* fetch SATA voltages */
    for (ii=0; ii<3; ii++) {
        if (ii==0)
            msg_info("Output 12v: ");
        if (ii==1)
            msg_info("Output 5v: ");
        if (ii==2)
            msg_info("Output 3.3v: ");

        rr = dev->driver->power.voltage(dev, handle, ii, &output_volts);
        msg_info("%5.2f V\n", output_volts);
    }

    if (flags.set_led == 1)
    {
        switch(settings.led_mode)
        {
        case BLINK:
            rr = dev->driver->led.blink(dev, handle,
                        settings.led_change_speed,
                        settings.led_count,
                        &settings.led_color[0],
                        &settings.led_color[1],
                        &settings.led_color[2],
                        &settings.led_color[3],
                        &settings.led_color[4],
                        &settings.led_color[5],
                        &settings.led_color[6]);
            break;
        case PULSE:
            rr = dev->driver->led.color_pulse(dev, handle,
                        settings.led_change_speed,
                        settings.led_count,
                        &settings.led_color[0],
                        &settings.led_color[1],
                        &settings.led_color[2],
                        &settings.led_color[3],
                        &settings.led_color[4],
                        &settings.led_color[5],
                        &settings.led_color[6]);
            break;
        case SHIFT:
            rr = dev->driver->led.color_shift(dev, handle,
                        settings.led_change_speed,
                        settings.led_count,
                        &settings.led_color[0],
                        &settings.led_color[1],
                        &settings.led_color[2],
                        &settings.led_color[3],
                        &settings.led_color[4],
                        &settings.led_color[5],
                        &settings.led_color[6]);
            break;
        case RAINBOW:
            rr = dev->driver->led.rainbow(dev, handle,
                        settings.led_change_speed);
            break;
        case TEMPERATURE:
            rr = dev->driver->led.temperature(dev, handle,
                        &settings.led_temperatures,
                        &settings.led_color[0], // Good LED Color
                        &settings.led_color[1], // Caution LED Color
                        &settings.led_color[2]); // Warning LED Color
            
            break;
        case STATIC:
        default:
            rr = dev->driver->led.static_color(dev, handle, &settings.led_color[0]);
            break;
        }
    }

    rr = dev->driver->deinit(handle, dev->write_endpoint);

    return 0;
}
