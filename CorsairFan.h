#ifndef _CORSAIRFAN_H
#define _CORSAIRFAN_H

class CorsairFan {
	public:
		unsigned int CommandId;
		CorsairFan();
		~CorsairFan();
		struct CorsairFanInfo {
			char* Name;
			int RPM;
			int Mode;
		} fanInfo[32];
		int num_fans;
		int ConnectedFans();
		void ReadFanInfo(int fanIndex, CorsairFanInfo fan);
		int SetFansInfo(int fanIndex, CorsairFanInfo fan);
		void PrintInfo(CorsairFanInfo fan);
		static char* GetFanModeString(int mode);
};

#endif
