#ifndef _CORSAIRTEMP_H
#define _CORSAIRTEMP_H
/*
* 0c 	RW 		1 byte 		Select active temperature sensor
* 0d 	R 		1 byte 		Number of temperature sensors
* 0e 	R 		2 bytes 	Temperature as measured by selected sensor
* 0f 	RW 		2 bytes 	Temperature limit (when the temperature goes over this, status is set to 0xff) 
*/
class CorsairTemp {
	public:
		CorsairTemp();
		~CorsairTemp();

	private:
		//static char* GetLedModeString(int mode);

	public:
		int GetTempSensors();
		int GetTemp(int);
		int GetTempLimit(int);
		int SetTempLimit(int);

};

#endif
