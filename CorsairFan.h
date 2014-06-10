#include <iostream>
#include <sstream>
#include <iomanip>

using std::endl;

class CorsairFan {
	public:
		CorsairFanInfo();
		~CorsairFanInfo();
		int ConnectedFans();
		void ReadFansInfo();
		int SetFansInfo(int fanIndex, CorsairFanInfo fanInfo);
		std::string Name;
		int RPM;
		int Mode;
		void PrintInfo();
		static std::string GetFanModeString(int mode);
};
