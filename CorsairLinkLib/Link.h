/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2014  Sean Nelson <audiohacked@gmail.com>

 * OpenCorsairLink is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * any later version.

 * OpenCorsairLink is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CORSAIRLINK_H
#define _CORSAIRLINK_H

#include <hidapi/hidapi.h>

#define MAX_STR 255

class CorsairLink {
	public:
		CorsairLink();
		~CorsairLink();
		int Initialize();
		void Close();

		// Protocol Functions
		int GetDeviceId();
		int GetFirmwareVersion();
		int GetProductName(char*);
		int GetDeviceStatus();

		// USB HID Functions
		char* _GetManufacturer();
		char* _GetProduct();

		// HID Wrappers
		int hid_read_wrapper(hid_device *handle, unsigned char *buf);
		int hid_wrapper(hid_device *handle, unsigned char *buf, size_t buf_size);

		//public variables
		hid_device *handle;
		unsigned char buf[256];
		unsigned int CommandId;

	private:
		int deviceId;
		struct hid_device_info;
		int max_ms_read_wait;
		
		void sleep(int ms);
};

#endif
