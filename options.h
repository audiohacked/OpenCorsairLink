#ifndef _OPTIONS_H
#define _OPTIONS_H

int parseArguments(int argc, char **argv, 
	int &fanNumber, int &fanMode, int &fanRPM,
	int &ledNumber, int &ledMode, CorsairLed::CorsairLedColor *leds);

#endif