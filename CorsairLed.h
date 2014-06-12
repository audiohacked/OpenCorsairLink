#ifndef _CORSAIRLED_H
#define _CORSAIRLED_H

#include <stdint.h>

class CorsairLed {
	private:
		//static char* GetLedModeString(int mode);
	public:
		CorsairLed();
		~CorsairLed();
		
		struct CorsairLedColor {
			int red;
			int green;
			int blue;
		} color[4];
		
		int SelectLed(int);
		int GetLedCount();
		
		int GetMode();
		int GetColor(CorsairLedColor*);
		int GetTempControlledMode();
		int Get_TempMode_Temp();
		int Get_TempMode_Color();
		int GetLedCycleColors(CorsairLedColor*);
		
		int SetMode(int);
		//int SetColor(int, int, int, int);
		int SetTempControlledMode();
		int Set_TempMode_Temp(int, int, int);
		int Set_TempMode_Color(CorsairLedColor *);
		int SetLedCycleColors(CorsairLedColor *);
};

#endif
