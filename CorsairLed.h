#ifndef _CORSAIRLED_H
#define _CORSAIRLED_H

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
		} led, leds[4];
		
		int GetLedCount();
		
		int GetMode(int ledIndex);
		int GetColor(int ledIndex, CorsairLedColor led);
		int GetTempControlledMode(int ledIndex);
		int Get_TempMode_Temp(int ledIndex);
		int Get_TempMode_Color(int ledIndex);
		int GetLedCycleColors(int ledIndex);
		
		int SetMode(int ledIndex, int mode);
		int SetColor(int ledIndex, int red, int green, int blue);
		int SetTempControlledMode(int ledIndex);
		int Set_TempMode_Temp(int ledIndex, int temp1, int temp2, int temp3);
		int Set_TempMode_Color(int ledIndex, CorsairLedColor *leds);
		int SetLedCycleColors(int ledIndex);
};

#endif
