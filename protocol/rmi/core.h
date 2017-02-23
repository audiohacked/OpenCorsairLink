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

#ifndef _PROTOCOL_RMI_H
#define _PROTOCOL_RMI_H

#include "../../common.h"

int corsairlink_rmi_device_id(struct corsair_device_info *dev);
int corsairlink_rmi_firmware_id(struct corsair_device_info *dev);
int corsairlink_rmi_channel_id(struct corsair_device_info *dev);
int corsairlink_rmi_product_id(struct corsair_device_info *dev);

int corsairlink_rmi_output_select(struct corsair_device_info *dev, uint8_t select);
int corsairlink_rmi_output_voltage(struct corsair_device_info *dev, float volts);
int corsairlink_rmi_output_amps(struct corsair_device_info *dev, float amps);
int corsairlink_rmi_output_watts(struct corsair_device_info *dev, float watts);

int corsairlink_rmi_total_watts(struct corsair_device_info *dev, float watts);
int corsairlink_rmi_supply_volts(struct corsair_device_info *dev, float volts);

int corsairlink_rmi_temp1(struct corsair_device_info *dev, float temp);
int corsairlink_rmi_temp2(struct corsair_device_info *dev, float temp);

// int corsairlink_rmi_fan_mode();
// int corsairlink_rmi_fan_pwm();
// int corsairlink_rmi_fan_pwm_percent();
// int corsairlink_rmi_fan_status();

#endif
