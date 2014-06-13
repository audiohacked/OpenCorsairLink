#ifndef _OPTIONS_H
#define _OPTIONS_H

int parseArguments(int argc, char **argv, int &info, 
	int &fanNumber, int &fanHighNumber, int &fanMode, int &fanPWM, int &fanRPM, int &fanThreshold,
	int &ledNumber, int &ledMode, CorsairLed::CorsairLedColor *leds,
	int &tempNumber);

#endif
