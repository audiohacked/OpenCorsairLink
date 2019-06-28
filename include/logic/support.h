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

#ifndef _SUPPORT_H
#define _SUPPORT_H

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

enum led_modes {
  STATIC = 0,
  BLINK = 1,
  PULSE = 2,
  SHIFT = 3,
  RAINBOW = 4,
  TEMPERATURE = 5,
};

struct temp_speed_pair {
  /** temperature */
  int8_t temperature;

  /** fan speed PWM */
  int8_t speed;
};

enum motor_modes {
  PWM = 0,
  RPM = 1,
  DEFAULT = 2,
  QUIET = 3,
  BALANCED = 4,
  PERFORMANCE = 5,
  CUSTOM = 6,
  UNDEFINED = 7,
};

#define INIT_WARNING_LED(xx)                                                   \
  xx.red = 0xFF;                                                               \
  xx.green = 0x00;                                                             \
  xx.blue = 0x00;

#define INIT_DEFAULT_LED(xx)                                                   \
  xx.red = 0xFF;                                                               \
  xx.green = 0xFF;                                                             \
  xx.blue = 0x00;

#define INIT_RAINBOW_LED(xx)                                                   \
  xx[0].red = 0xff;                                                            \
  xx[0].green = 0x00;                                                          \
  xx[0].blue = 0x00;                                                           \
  xx[1].red = 0xff;                                                            \
  xx[1].green = 0x80;                                                          \
  xx[1].blue = 0x00;                                                           \
  xx[2].red = 0xff;                                                            \
  xx[2].green = 0xff;                                                          \
  xx[2].blue = 0x00;                                                           \
  xx[3].red = 0x00;                                                            \
  xx[3].green = 0xff;                                                          \
  xx[3].blue = 0x00;                                                           \
  xx[4].red = 0x00;                                                            \
  xx[4].green = 0x00;                                                          \
  xx[4].blue = 0xff;                                                           \
  xx[5].red = 0x4b;                                                            \
  xx[5].green = 0x00;                                                          \
  xx[5].blue = 0x82;                                                           \
  xx[6].red = 0x7f;                                                            \
  xx[6].green = 0x00;                                                          \
  xx[6].blue = 0xff;

#endif
