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

#ifndef _OPTIONS_H
#define _OPTIONS_H

#include <stdint.h>
#include "common.h"

struct option_flags {
    // flags
    //unsigned int debug_messages:1;
    unsigned int set_led:1;
    unsigned int set_temperature:1;
    unsigned int set_fan:1;
    unsigned int set_pump:1;
    unsigned int device_firmware:1;
    unsigned int read_temperature:1;
    unsigned int read_fan_speed:1;
};

struct option_parse_return {
    uint8_t led_mode;
    struct color led_color[7];
    int8_t warning_led_temp;
    struct color warning_led;
    uint8_t fan;
    uint8_t fan_mode;
    uint16_t fan_data;
    struct fan_table fan1;
    struct fan_table pump;
    uint8_t pump_mode;
};

int options_parse(int argc, char **argv,
    struct option_flags *flags,
    int8_t *device_number,
    struct option_parse_return *settings);

#endif
