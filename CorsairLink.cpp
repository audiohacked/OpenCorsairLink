#include <stdlib.h>
#include "CorsairLink.h"

CorsairLink::CorsairLink() {
	handle = NULL;
	CommandId = 0x81;
	max_ms_read_wait = 5000;
	fans = new CorsairFanInfo[5];
}

int CorsairLink::Initialize() {
	if(handle == NULL){
		if (hid_init())
			return 0;
			
		// Set up the command buffer.
		unsigned char buf[256];
		memset(buf,0x00,sizeof(buf));
		buf[0] = 0x01;
		buf[1] = 0x81;
		
		// Open the device using the VID, PID,
		// and optionally the Serial number.
		// open Corsair H80i or H100i cooler 
		handle = hid_open(0x1b1c, 0x0c04, NULL);
		if (!handle) {
			std::cerr << "Error: Unable to open Corsair H80i or H100i CPU Cooler\n";
			return 0;
		}
		hid_set_nonblocking(handle, 1);
		
		memset(buf,0,sizeof(buf));

		// Read Device ID: 0x3b = H80i. 0x3c = H100i
		buf[0] = 0x03; // Length
		buf[1] = this->CommandId++; // Command ID
		buf[2] = ReadOneByte; // Command Opcode
		buf[3] = DeviceID; // Command data...
		buf[4] = 0x00;
		
		int res = hid_write(handle, buf, 17);
		if (res < 0) {
			std::cerr << "Error: Unable to write() " << hid_error(handle) << endl;
		}

		hid_read_wrapper(handle, buf);
		if (res < 0) {
			std::cerr << "Error: Unable to read() " << hid_error(handle) << endl;
		}

		int deviceId = buf[2];
		
		if ((deviceId != 0x3b) && (deviceId != 0x3c)) {
			std::cerr <<  "Device ID: %02hhx mismatch. Not Corsair H80i or H100i CPU Cooler" << buf[2] << endl;
			this->Close();
			return 0;
		}
	}
	else {
		std::cerr << "Cannot initialize twice" << endl;
		return 0;
	}
	return 1;
}

int CorsairLink::ConnectedFans() {
	int fans = 0, i = 0, fanMode = 0;
	unsigned char buf[256];
	
	for (i = 0; i < 5; i++) {
		memset(buf,0x00,sizeof(buf));
		// Read fan Mode
		buf[0] = 0x07; // Length
		buf[1] = this->CommandId++; // Command ID
		buf[2] = WriteOneByte; // Command Opcode
		buf[3] = FAN_Select; // Command data...
		buf[4] = i; // select fan
		buf[5] = this->CommandId++; // Command ID
		buf[6] = ReadOneByte; // Command Opcode
		buf[7] = FAN_Mode; // Command data...
		
		int res = hid_write(handle, buf, 11);
		if (res < 0) {
			std::cerr << "Error: Unable to write() " << hid_error(handle) << endl;
		}

		res = hid_read_wrapper(handle, buf);
		if (res < 0) {
			std::cerr << "Error: Unable to read() " << hid_error(handle) << endl;
		}
		fanMode = buf[4];
		
		if(fanMode != 0x03){
			fans++;
		}
	}
	
	return fans;
}

void CorsairLink::ReadFansInfo(){
	int i = 0, fanMode = 0, res = 0;
	unsigned char buf[256];
	std::ostringstream sstream;
	for (i = 0; i < 5; i++) {
		sstream.str("");
		sstream.clear();
		
		if(i < 4){
			sstream << "Fan " << i + 1;
		}
		else {
			sstream << "Pump";
		}
		this->fans[i].Name = sstream.str();
			
		memset(buf,0x00,sizeof(buf));
		// Read fan Mode
		buf[0] = 0x07; // Length
		buf[1] = this->CommandId++; // Command ID
		buf[2] = WriteOneByte; // Command Opcode
		buf[3] = FAN_Select; // Command data...
		buf[4] = i; // select fan
		buf[5] = this->CommandId++; // Command ID
		buf[6] = ReadOneByte; // Command Opcode
		buf[7] = FAN_Mode; // Command data...
		
		res = hid_write(handle, buf, 11);
		if (res < 0) {
			std::cerr << "Error: Unable to write() " << hid_error(handle) << endl;
		}

		res = hid_read_wrapper(handle, buf);
		if (res < 0) {
			std::cerr << "Error: Unable to read() " << hid_error(handle) << endl;
		}
		fanMode = buf[4] & 0x0E;
		
		memset(buf,0x00,sizeof(buf));
		// Read fan RPM
		buf[0] = 0x07; // Length
		buf[1] = this->CommandId++; // Command ID
		buf[2] = WriteOneByte; // Command Opcode
		buf[3] = FAN_Select; // Command data...
		buf[4] = i; // select fan
		buf[5] = this->CommandId++; // Command ID
		buf[6] = ReadTwoBytes; // Command Opcode
		buf[7] = FAN_ReadRPM; // Command data...

		res = hid_write(handle, buf, 11);
		if (res < 0) {
			std::cerr << "Error: Unable to write() " << hid_error(handle) << endl;
		}

		res = hid_read_wrapper(handle, buf);
		//All data is little-endian.
		int rpm = buf[5] << 8;
		rpm += buf[4];

		this->fans[i].Mode = fanMode;
		this->fans[i].RPM = rpm;

	}
}

int CorsairLink::SetFansInfo(int fanIndex, CorsairFanInfo fanInfo){
	unsigned char buf[256];
	memset(buf,0x00,sizeof(buf));

	if(fanInfo.Mode == FixedPWM || fanInfo.Mode == FixedRPM
		|| fanInfo.Mode == Default || fanInfo.Mode == Quiet
		|| fanInfo.Mode == Balanced	|| fanInfo.Mode == Performance
		|| fanInfo.Mode == Custom) {

		buf[0] = 0x0b; // Length
		buf[1] = this->CommandId++; // Command ID
		buf[2] = WriteOneByte; // Command Opcode
		buf[3] = FAN_Select; // Command data...
		buf[4] = fanIndex; // select fan
		buf[5] = this->CommandId++; // Command ID
		buf[6] = WriteOneByte; // Command Opcode
		buf[7] = FAN_Mode; // Command data...
		buf[8] = fanInfo.Mode;
		buf[9] = this->CommandId++; // Command ID
		buf[10] = ReadOneByte; // Command Opcode
		buf[11] = FAN_Mode; // Command data...

		int res = hid_write(this->handle, buf, 17);
		if (res < 0) {
			std::cerr << hid_error(handle);
			return 1;
		}

		res = hid_read_wrapper(handle, buf);
		if (res < 0) {
			std::cerr << hid_error(handle);
			return 1;
		}
		if(fanInfo.Mode != buf[6]){
			std::cerr << "Cannot set fan mode." << endl;
			return 1;
		}
	} else {
		std::cerr << "Invalid fan mode." << endl;
		return 1;
	}
	if(fanInfo.RPM != 0) {
		memset(buf,0x00,sizeof(buf));

		buf[0] = 0x0b; // Length
		buf[1] = this->CommandId++; // Command ID
		buf[2] = WriteOneByte; // Command Opcode
		buf[3] = FAN_Select; // Command data...
		buf[4] = fanIndex; // select fan
		buf[5] = this->CommandId++; // Command ID
		buf[6] = WriteTwoBytes; // Command Opcode
		buf[7] = FAN_FixedRPM; // Command data...
		buf[8] = fanInfo.RPM & 0x00FF;
		buf[9] = fanInfo.RPM >> 8;
		buf[10] = this->CommandId++; // Command ID
		buf[11] = ReadTwoBytes; // Command Opcode
		buf[12] = FAN_ReadRPM; // Command data...

		int res = hid_write(this->handle, buf, 18);
		if (res < 0) {
			std::cerr << hid_error(handle);
			return 1;
		}

		res = hid_read_wrapper(handle, buf);
		if (res < 0) {
			std::cerr << hid_error(handle);
			return 1;
		}
		//All data is little-endian.
		int rpm = buf[5] << 8;
		rpm += buf[4];
		if(fanInfo.RPM != rpm){
			std::cerr << "Cannot set fan RPM.";
			return 1;
		}
	}

	return 0;
}

void CorsairLink::Close() {
	if(handle != NULL){	
		hid_close(handle);
		hid_exit();
		handle = NULL;
	}
}

std::wstring CorsairLink::GetManufacturer(){
	std::wstring str;
	wchar_t wstr[MAX_STR];
	wstr[0] = 0x0000;
	int res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	
	if (res < 0)
		std::cerr << "Unable to read manufacturer string\n";
	str = wstr;
	return str;
}

std::wstring CorsairLink::GetProduct(){
	std::wstring str;
	wchar_t wstr[MAX_STR];
	wstr[0] = 0x0000;
	
	int res = hid_get_product_string(handle, wstr, MAX_STR);
	if (res < 0)
		std::cerr << "Unable to read product string\n";
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
	while (res == 0 && sleepTotal < this->max_ms_read_wait) {
		res = hid_read(handle, buf, sizeof(buf));
		if (res < 0)
			std::cerr << "Unable to read()\n";
		
		this->sleep(100);
		sleepTotal += 100;
	}
	if(sleepTotal == this->max_ms_read_wait) {
		res = 0;
	}
	return 1;
}

void CorsairLink::sleep(int ms){
	#ifdef WIN32
	Sleep(ms);
	#else
	usleep(ms*1000);
	#endif
}

CorsairLink::~CorsairLink() {
	this->Close();
//	if(fans != NULL) {
//		free(fans);
//	}
}
