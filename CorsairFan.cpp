#include "CorsairFanInfo.h"

CorsairFan::CorsairFan(){
	this->RPM = 0;
	this->Mode = 0x03;
}

void CorsairFan::PrintInfo(){
	std::cout << this->Name << ":" << endl;
	std::ios_base::fmtflags oldFlags = std::cout.flags();
	std::cout.flags ( std::ios::right | std::ios::hex | std::ios::showbase );
	std::cout << "\tMode: " << CorsairFanInfo::GetFanModeString(this->Mode) << endl;
	std::cout.flags(oldFlags);
	std::cout << "\tRPM: " << this->RPM <<endl;
}

std::string CorsairFan::GetFanModeString(int mode){
	std::string modeString = "";
	
	switch(mode){
		case FixedPWM:
			modeString = "Fixed PWM";
			break;
		case FixedRPM:
			modeString = "Fixed RPM";
			break;
		case Default:
			modeString = "Default";
			break;
		case Quiet:
			modeString = "Quiet";
			break;
		case Balanced:
			modeString = "Balanced";
			break;
		case Performance:
			modeString = "Performance";
			break;
		case Custom:
			modeString = "Custom";
			break;
		default:
			std::ostringstream sstream;
			sstream << "N/A (" << std::hex << mode << ")";
			modeString = sstream.str();
			break;
	}
	
	return modeString;
}

int CorsairFan::ConnectedFans() {
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

void CorsairFan::ReadFansInfo(){
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
			fprintf(stderr, "Error: Unable to write() %s\n", (char*)hid_error(handle) );
		}

		res = hid_read_wrapper(handle, buf);
		if (res < 0) {
			fprintf(stderr, "Error: Unable to read() %s\n", (char*)hid_error(handle) );
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
			fprintf(stderr, "Error: Unable to write() %s\n", hid_error(handle) );
		}

		res = hid_read_wrapper(handle, buf);
		//All data is little-endian.
		int rpm = buf[5] << 8;
		rpm += buf[4];

		//this->fans[i].Mode = fanMode;
		//this->fans[i].RPM = rpm;

	}
}

int CorsairFan::SetFansInfo(int fanIndex, CorsairFanInfo fanInfo){
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

CorsairFan::~CorsairFan(){
	
}
