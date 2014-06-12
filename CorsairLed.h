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
