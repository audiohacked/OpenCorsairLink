#ifndef _CORSAIRFAN_H
#define _CORSAIRFAN_H

class CorsairFan {
	public:
		CorsairFan(){}
		~CorsairFan(){}

		struct CorsairFanInfo {
			char* Name;
			int RPM;
			int Mode;
		} fanInfo[32];

		int num_fans;
		int ConnectedFans();
		void ReadFanInfo(int fanIndex, CorsairFanInfo *fan);
		int SetFansInfo(int fanIndex, CorsairFanInfo fan);
		void PrintInfo(CorsairFanInfo fan);
		static char* GetFanModeString(int mode);

	public:
		int SelectFan(int index);
		int CurrentFan();
		int GetFanCount();

		int GetFanMode();
		int SetFanMode(int mode);

		int GetFanPWM();
		int SetFanPWM(int pwm);

		int GetFanRPM();
		int SetFanRPM(int rpm);

		int GetExternalReport();
		int SetExternalReport();

		int GetCurrentRPM();
		int GetMaxRPM();

		int GetFanUnderspeedThreshold();
		int SetFanUnderspeedThreshold(int threshold);

		int GetRPMTable(int&, int&, int&, int&, int&);
		int SetRPMTable(int, int, int, int, int);

		int GetTemperatureTable(int&, int&, int&, int&, int&);
		int SetTemperatureTable(int, int, int, int, int);
};

#endif
