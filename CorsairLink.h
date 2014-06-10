#ifndef _CORSAIRLINK_H
#define _CORSAIRLINK_H

#include <hidapi/hidapi.h>

#define MAX_STR 255

class CorsairLink {
	public:
		//CorsairFan *fans;
		CorsairLink();
		~CorsairLink();
		int Initialize();
		void Close();

		// Protocol Functions
		int GetDeviceId();

		// USB HID Functions
		char* _GetManufacturer();
		char* _GetProduct();

		// HID Wrappers
		int hid_read_wrapper(hid_device *handle, unsigned char *buf);

	private:
		int deviceId;
		unsigned char buf[256];
		struct hid_device_info;
		unsigned int CommandId;
		int max_ms_read_wait;
		
		void sleep(int ms);
};

hid_device *handle;

#endif