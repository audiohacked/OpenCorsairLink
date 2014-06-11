#ifndef _CORSAIRLED_H
#define _CORSAIRLED_H

class CorsairLed {
	private:
		struct CorsairLedColor {
			int red;
			int green;
			int blue;
		} led;
		//static char* GetLedModeString(int mode);
	public:
		CorsairLed();
		~CorsairLed();
		
		int GetLedCount();
		
		int GetMode(int ledIndex);
		int GetColor(int ledIndex, CorsairLedColor led);
		int GetTempControlMode(int ledIndex);
		int Get_TempMode_Temp(int ledIndex);
		int Get_TempMode_Color(int ledIndex);
		int GetLedCycleColors(int ledIndex);
		
		int SetMode(int ledIndex, int mode);
		int SetColor(int ledIndex, int red, int green, int blue);
		int SetTempControlMode(int ledIndex);
		int Set_TempMode_Temp(int ledIndex);
		int Set_TempMode_Color(int ledIndex);
		int SetLedCycleColors(int ledIndex);
};

#endif
