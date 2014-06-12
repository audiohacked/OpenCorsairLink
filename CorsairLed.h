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
			uint8_t red;
			uint8_t green;
			uint8_t blue;
		} color[4];
		
		int GetLedCount();
		
		int GetMode(int);
		int GetColor(int, CorsairLedColor*);
		int GetTempControlledMode(int);
		int Get_TempMode_Temp(int);
		int Get_TempMode_Color(int);
		int GetLedCycleColors(int);
		
		int SetMode(int, int);
		//int SetColor(int, int, int, int);
		int SetTempControlledMode(int);
		int Set_TempMode_Temp(int, int, int, int);
		int Set_TempMode_Color(int, CorsairLedColor *);
		int SetLedCycleColors(int,CorsairLedColor *);
};

#endif
