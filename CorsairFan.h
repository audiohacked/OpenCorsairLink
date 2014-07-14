/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2014  Sean Nelson <audiohacked@gmail.com>

 * OpenCorsairLink is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * any later version.

 * OpenCorsairLink is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

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
