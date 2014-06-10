#include <stdio.h>

using std::endl;

class CorsairFan {
	public:
		CorsairFan();
		~CorsairFan();
		struct CorsairFanInfo {
			char* Name;
			int RPM;
			int Mode;
		} fanInfo[32];
		int num_fans;
		int ConnectedFans();
		void ReadFansInfo();
		int SetFansInfo(int fanIndex, CorsairFanInfo fanInfo);
		void PrintInfo(CorsairFanInfo fan);
		static char* GetFanModeString(int mode);
};
