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

#ifndef _COMMON_H
#define _COMMON_H

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define MAJOR 0 // Full Rewrite
#define MINOR 6 // Major API Change
#define PATCH_MAJOR 0 // Backward-compatible API Change
#define PATCH_MINOR 0 // Patch/Fix Change

#define VERSION "v"STR(MAJOR)"."STR(MINOR)"."STR(PATCH_MAJOR)"."STR(PATCH_MINOR)

struct color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct led_temperatures {
    uint8_t temp1;
    uint8_t temp2;
    uint8_t temp3;
};

struct fan_table {
    /** temperatures */
    int8_t t1;
    int8_t t2;
    int8_t t3;
    int8_t t4;
    int8_t t5;
    int8_t t6;

    /** fan speeds */
    int8_t s1;
    int8_t s2;
    int8_t s3;
    int8_t s4;
    int8_t s5;
    int8_t s6;
};

enum pump_modes {
    PWM = 0,
    RPM = 1,
    DEFAULT = 2,
    QUIET = 3,
    BALANCED = 4,
    PERFORMANCE = 5,
    CUSTOM = 6,
    UNDEFINED = 7,
};

#endif /* _COMMON_H */
