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
