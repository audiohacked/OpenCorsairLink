#include "CorsairFanInfo.h"

CorsairFanInfo::CorsairFanInfo(){
	this->RPM = 0;
	this->Mode = 0x03;
}

void CorsairFanInfo::PrintInfo(){
	std::cout << this->Name << ":" << endl;
	std::ios_base::fmtflags oldFlags = std::cout.flags();
	std::cout.flags ( std::ios::right | std::ios::hex | std::ios::showbase );
	std::cout << "\tMode: " << CorsairFanInfo::GetFanModeString(this->Mode) << endl;
	std::cout.flags(oldFlags);
	std::cout << "\tRPM: " << this->RPM <<endl;
}

std::string CorsairFanInfo::GetFanModeString(int mode){
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

CorsairFanInfo::~CorsairFanInfo(){
	
}
