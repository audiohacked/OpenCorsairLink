#include <iostream>
#include <sstream>
#include <iomanip>

using std::endl;

class CorsairFan {
	public:
		CorsairFan();
		~CorsairFan();
		int ConnectedFans();
		void ReadFansInfo();
		int SetFansInfo(int fanIndex, CorsairFanInfo fanInfo);
		char* Name;
		void PrintInfo();
		static char* GetFanModeString(int mode);
	private:
		struct CorsairFanInfo {
			int RPM;
			int Mode;
		};
};
