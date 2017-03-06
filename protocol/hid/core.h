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

#ifndef _PROTOCOL_HID_H
#define _PROTOCOL_HID_H

#include "../../common.h"

enum HID_Commands {
	DeviceID = 0x00, 
	FirmwareID = 0x01,
	ProductName = 0x02,
	Status = 0x03,
	LED_SelectCurrent = 0x04,
	LED_Count = 0x05,
	LED_Mode = 0x06,
	LED_CurrentColor = 0x07,
	LED_TemperatureColor = 0x08,
	LED_TemperatureMode = 0x09,
	LED_TemperatureModeColors = 0x0A,
	LED_CycleColors = 0x0B,
	TEMP_SelectActiveSensor = 0x0C,
	TEMP_CountSensors = 0x0D,
	TEMP_Read = 0x0E,
	TEMP_Limit = 0x0F,
	FAN_Select = 0x10,
	FAN_Count = 0x11,
	FAN_Mode = 0x12,
	FAN_FixedPWM = 0x13,
	FAN_FixedRPM = 0x14,
	FAN_ReportExtTemp = 0x15,
	FAN_ReadRPM = 0x16,
	FAN_MaxRecordedRPM = 0x17,
	FAN_UnderSpeedThreshold = 0x18,
	FAN_RPMTable = 0x19,
	FAN_TempTable = 0x1A
};

enum HID_Op_Codes{
	WriteOneByte = 0x06,
	ReadOneByte = 0x07,
	WriteTwoBytes = 0x08,
	ReadTwoBytes = 0x09,
	WriteThreeBytes = 0x0A,
	ReadThreeBytes = 0x0B,
};

enum HID_Led_Modes {
	StaticColor = 0x00,
	TwoColorCycle = 0x40,
	FourColorCycle = 0x80,
	TemperatureColor = 0xC0
};

enum HID_Fan_Modes {
	HID_FixedPWM = 0x02,
	HID_FixedRPM = 0x04,
	HID_Default = 0x06,
	HID_Quiet = 0x08,
	HID_Balanced = 0x0A,
	HID_Performance = 0x0C,
	HID_Custom = 0x0E
};

static uint8_t CommandId = 0x81;

int corsairlink_hid_device_id(struct corsair_device_info *dev);
int corsairlink_hid_name(struct corsair_device_info *dev, char *name);
int corsairlink_hid_vendor(struct corsair_device_info *dev, char *name);
int corsairlink_hid_product(struct corsair_device_info *dev, char *name);
int corsairlink_hid_firmware_id(struct corsair_device_info *dev, char *firmware);

int corsairlink_hid_change_led(struct corsair_device_info *dev,
			struct color *color_led, struct color *warning_led,
			uint8_t Warning_Temp, uint8_t Warning_Enable);

int corsairlink_hid_temperature(struct corsair_device_info *dev,
			uint8_t selector, uint16_t *temperature);

#endif
