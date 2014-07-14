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

#ifndef _CORSAIRLED_H
#define _CORSAIRLED_H

#include <stdint.h>

class CorsairLed {
	private:
		//static char* GetLedModeString(int mode);
	public:
		struct CorsairLedColor {
			int red;
			int green;
			int blue;
		} color[4];

		int color_set_by_opts;
		int color_set_by_func;

		int SelectLed(int);
		int GetLedCount();
		
		int GetMode();
		int GetColor(CorsairLedColor*);
		int GetTempColor(CorsairLedColor *);
		int Get_TempMode_Temp(int &, int &, int &);
		int Get_TempMode_Color(CorsairLedColor *);
		int GetLedCycleColors(CorsairLedColor *);
		
		int SetMode(int);
		int SetTempColor(CorsairLedColor *);
		int Set_TempMode_Temp(int, int, int);
		int Set_TempMode_Color(CorsairLedColor *);
		int SetLedCycleColors(CorsairLedColor *);

		CorsairLed()
		{
			color_set_by_opts = 0;
			color_set_by_func = 0;
		}
	
		~CorsairLed()
		{
		}
};

#endif
