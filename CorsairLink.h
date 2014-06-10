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
		
		std::wstring GetManufacturer();
		std::wstring GetProduct();
		
	private:
		hid_device *handle;
		struct hid_device_info;
		unsigned int CommandId;
		int max_ms_read_wait;
		
		int hid_read_wrapper(hid_device *handle, unsigned char *buf);
		void sleep(int ms);
};
