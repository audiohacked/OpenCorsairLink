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

#define _XOPEN_SOURCE 500

#include <getopt.h>
#include <stdint.h>

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

struct option_flags {
  // flags
  // unsigned int debug_messages:1;
  unsigned int set_led : 1;
  unsigned int set_temperature : 1;
  unsigned int set_fan : 1;
  unsigned int set_pump : 1;
};

struct led_control {
  uint8_t channel;
  enum led_modes mode;
  struct color led_colors[7];
  uint16_t temperatures[3];
  uint8_t speed;
  uint8_t count;
};

struct fan_control {
  uint8_t channel;
  enum motor_modes mode;
  struct temp_speed_pair table[6];

  // data returns
  uint8_t fan_count;
  uint8_t fan_type;
  uint8_t temp_sensor_id;
  uint16_t data;
  uint16_t speed;
  uint8_t speed_pwm;
  uint16_t speed_rpm;
  uint16_t max_speed;
  char mode_string[64];
};

struct pump_control {
  uint8_t channel;
  enum motor_modes mode;
  struct temp_speed_pair table[6];

  // data returns
  uint16_t speed;
  uint8_t speed_pwm;
  uint16_t speed_rpm;
  uint16_t max_speed;
};

struct option_parse_return {
  struct led_control led_ctrl;
  struct fan_control fan_ctrl;
  struct pump_control pump_ctrl;
};

enum {
  OPTION_HELP = 0,
  OPTION_VERSION,
  OPTION_DEBUG,
  OPTION_DUMP,
  OPTION_MACHINE,
  OPTION_DEVICE,
  OPTION_FAN,
  OPTION_LED,
  OPTION_PUMP,
};

static struct option long_options[] = {
    {"help", no_argument, 0, OPTION_HELP},
    {"version", no_argument, 0, OPTION_VERSION},
    {"debug", no_argument, 0, OPTION_DEBUG},
    {"dump", no_argument, 0, OPTION_DUMP},
    {"machine", no_argument, 0, OPTION_MACHINE},
    {"device", required_argument, 0, OPTION_DEVICE},
    {"led", required_argument, 0, OPTION_LED},
    {"fan", required_argument, 0, OPTION_FAN},
    {"pump", required_argument, 0, OPTION_PUMP},
    {0, 0, 0, 0}};

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

enum {
  SUBOPTION_FAN_CHANNEL = 0,
  SUBOPTION_FAN_MODE,
  SUBOPTION_FAN_PWM,
  SUBOPTION_FAN_RPM,
  SUBOPTION_FAN_TEMPERATURES,
  SUBOPTION_FAN_SPEEDS,
  SUBOPTION_FAN_LIST_END,
};

enum {
  SUBOPTION_LED_CHANNEL = 0,
  SUBOPTION_LED_MODE,
  SUBOPTION_LED_SPEED,
  SUBOPTION_LED_COLORS,
  SUBOPTION_LED_WARN,
  SUBOPTION_LED_TEMPERATURE,
  SUBOPTION_LED_LIST_END,
};

enum {
  SUBOPTION_PUMP_MODE = 0,
  SUBOPTION_PUMP_PWM,
  SUBOPTION_PUMP_RPM,
  SUBOPTION_PUMP_TEMPERATURES,
  SUBOPTION_PUMP_SPEEDS,
  SUBOPTION_PUMP_LIST_END,
};

static char *fan_options[] = {[SUBOPTION_FAN_CHANNEL] = "channel",
                              [SUBOPTION_FAN_MODE] = "mode",
                              [SUBOPTION_FAN_PWM] = "pwm",
                              [SUBOPTION_FAN_RPM] = "rpm",
                              [SUBOPTION_FAN_TEMPERATURES] = "temps",
                              [SUBOPTION_FAN_SPEEDS] = "speeds",
                              [SUBOPTION_FAN_LIST_END] = 0};

static char *led_options[] = {[SUBOPTION_LED_CHANNEL] = "channel",
                              [SUBOPTION_LED_MODE] = "mode",
                              [SUBOPTION_LED_SPEED] = "speed",
                              [SUBOPTION_LED_COLORS] = "colors",
                              [SUBOPTION_LED_WARN] = "warning_color",
                              [SUBOPTION_LED_TEMPERATURE] = "temperature",
                              [SUBOPTION_LED_LIST_END] = 0};

static char *pump_options[] = {[SUBOPTION_PUMP_MODE] = "mode",
                               [SUBOPTION_PUMP_PWM] = "pwm",
                               [SUBOPTION_PUMP_RPM] = "rpm",
                               [SUBOPTION_PUMP_TEMPERATURES] = "temps",
                               [SUBOPTION_PUMP_SPEEDS] = "speeds",
                               [SUBOPTION_PUMP_LIST_END] = 0};

void fan_suboptions_parse(char *subopts, struct fan_control *settings);
void led_suboptions_parse(char *subopts, struct led_control *settings);
void pump_suboptions_parse(char *subopts, struct pump_control *settings);

void fan_control_init(struct fan_control *settings);
void led_control_init(struct led_control *settings);
void pump_control_init(struct pump_control *settings);

void options_print(void);

int options_parse(int argc, char **argv, struct option_flags *flags,
                  int8_t *device_number, struct option_parse_return *settings);

#endif
