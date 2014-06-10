#include <stdlib.h>
#include "CorsairLink.h"

CorsairLink::CorsairLink() {
	handle = NULL;
	CommandId = 0x81;
	max_ms_read_wait = 5000;
	fans = new CorsairFan;
}

int CorsairLink::Initialize()
{
	if(handle == NULL)
	{
		if (hid_init())
			return 0;

		// Set up the command buffer.
		//memset(buf,0x00,sizeof(buf));
		//buf[0] = 0x01;
		//buf[1] = 0x81;

		// Open the device using the VID, PID,
		// and optionally the Serial number.
		// open Corsair H80i or H100i cooler 
		handle = hid_open(0x1b1c, 0x0c04, NULL);
		if (!handle)
		{
			fprintf(stderr, "Error: Unable to open Corsair H80i or H100i CPU Cooler\n");
			return 0;
		}
		hid_set_nonblocking(handle, 1);

		deviceId = this->GetDeviceId();
		if ((deviceId != 0x3b) && (deviceId != 0x3c))
		{
			fprintf(stderr, "Device ID: %02hhx mismatch. Not Corsair H80i or H100i CPU Cooler\n", deviceId );
			this->Close();
			return 0;
		}
	} else {
		fprintf(stderr, "Cannot initialize twice\n" );
		return 0;
	}
	return 1;
}

int CorsairLink::GetDeviceId(void)
{
	memset(buf,0,sizeof(buf));

	// Read Device ID: 0x3b = H80i. 0x3c = H100i
	buf[0] = 0x03; // Length
	buf[1] = this->CommandId++; // Command ID
	buf[2] = ReadOneByte; // Command Opcode
	buf[3] = DeviceID; // Command data...
	buf[4] = 0x00;

	int res = hid_write(handle, buf, 17);
	if (res < 0) {
		fprintf(stderr, "Error: Unable to write() %s\n", hid_error(handle) );
	}

	hid_read_wrapper(handle, buf);
	if (res < 0) {
		fprintf(stderr, "Error: Unable to read() %s\n", hid_error(handle) );
	}

	return buf[2];
}

int CorsairLink::GetFirmwareVersion()
{

}

char* CorsairLink::GetProductName()
{

}

char* CorsairLink::DeviceStatus()
{

}

char* CorsairLink::_GetManufacturer()
{
	char *str;
	char wstr[MAX_STR];
	wstr[0] = 0x0000;

	int res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	if (res < 0)
		fprintf(stderr, "Unable to read manufacturer string\n");

	str = wstr;
	return str;
}

char* CorsairLink::_GetProduct()
{
	char* str;
	char wstr[MAX_STR];
	wstr[0] = 0x0000;
	
	int res = hid_get_product_string(handle, wstr, MAX_STR);
	if (res < 0)
		fprintf(stderr, "Unable to read product string\n");

	str = wstr;
	return str;
}

int CorsairLink::hid_read_wrapper (hid_device *handle, unsigned char *buf)
{
	// Read requested state. hid_read() has been set to be
	// non-blocking by the call to hid_set_nonblocking() above.
	// This loop demonstrates the non-blocking nature of hid_read().
	int res = 0;
	int sleepTotal = 0;
	while (res == 0 && sleepTotal < this->max_ms_read_wait)
	{
		res = hid_read(handle, buf, sizeof(buf));
		if (res < 0)
			fprintf(stderr, "Unable to read()\n");
		
		this->sleep(100);
		sleepTotal += 100;
	}
	if(sleepTotal == this->max_ms_read_wait)
	{
		res = 0;
	}
	return 1;
}

void CorsairLink::sleep(int ms)
{
	#ifdef WIN32
	Sleep(ms);
	#else
	usleep(ms*1000);
	#endif
}

void CorsairLink::Close()
{
	if(handle != NULL)
	{
		hid_close(handle);
		hid_exit();
		handle = NULL;
	}
}

CorsairLink::~CorsairLink()
{
	this->Close();
//	if(fans != NULL) {
//		free(fans);
//	}
}
