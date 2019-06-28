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

#ifndef _SUBOPTIONS_H
#define _SUBOPTIONS_H

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
  SUBOPTION_LED_TEMPERATURES,
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
  struct temp_speed_pair table[7];

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
                              [SUBOPTION_LED_TEMPERATURES] = "temps",
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

#endif
