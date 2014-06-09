#include <iostream>
#include <sstream>
#include <iomanip>

using std::endl;

enum CorsairFanModes {
	FixedPWM = 0x02,
	FixedRPM = 0x04,
	Default = 0x06,
	Quiet = 0x08,
	Balanced = 0x0A,
	Performance = 0x0C,
	Custom = 0x0E
};

class CorsairFanInfo {
	public:
		CorsairFanInfo();
		~CorsairFanInfo();
		std::string Name;
		int RPM;
		int Mode;
		void PrintInfo();
		static std::string GetFanModeString(int mode);
};
