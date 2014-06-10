#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <hidapi/hidapi.h>
#include "CorsairLinkProto.h"
#include "CorsairFan.h"

using std::endl;

#define MAX_STR 255

class CorsairLink {
	public:
		CorsairFan *fans;
		CorsairLink();
		~CorsairLink();
		int Initialize();
		void Close();

		int GetDeviceId();

		char* _GetManufacturer();
		char* _GetProduct();
		
	private:
		int deviceId;
		hid_device *handle;
		unsigned char buf[256];
		struct hid_device_info;
		unsigned int CommandId;
		int max_ms_read_wait;
		
		int hid_read_wrapper(hid_device *handle, unsigned char *buf);
		void sleep(int ms);
};
