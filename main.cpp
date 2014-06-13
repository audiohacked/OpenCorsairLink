#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "CorsairLinkProto.h"
#include "CorsairLink.h"
#include "CorsairFan.h"
#include "CorsairLed.h"
#include "CorsairTemp.h"
#include "options.h"

CorsairLink *cl = new CorsairLink();
CorsairFan *f = new CorsairFan();
CorsairLed *l = new CorsairLed();
CorsairTemp *t = new CorsairTemp();

int main(int argc, char **argv) {
	//fprintf(stdout, "Open Corsair Link\n");

	int info = 0;
	int fanNumber = 0, fanMode = 0, fanPWM = 0, fanRPM = 0, fanThreshold = 0;
	int ledNumber = 0, ledMode = -1;
	int tempNumber = 0;

	if(parseArguments(argc, argv, info,
	   fanNumber, fanMode, fanPWM, fanRPM, fanThreshold,
	   ledNumber, ledMode, l->color,
	   tempNumber)) {
		return 1;
	}

	if(!cl->Initialize()) {
		fprintf(stdout, "Cannot initialize link.\n");
		delete cl;
		return 1;
	}

	if (info == 1) {
		int i = 0;
		char prod[9] = "";
		cl->GetProductName(prod);
		fprintf(stdout, "%s: Firmware v%02X\n\n", prod, cl->GetFirmwareVersion());
		if (ledNumber > 0) {
			fprintf(stdout, "Number of Controllable LEDs: %i\n", l->GetLedCount());
			fprintf(stdout, "LED Mode: %02X\n", l->GetMode());
			l->GetColor(&l->color[0]);
			fprintf(stdout, "LED Color:\n\tRed: %i\n\tGreen: %i\n\tBlue: %i\n", l->color[0].red, l->color[0].green, l->color[0].blue );
		}
		if (tempNumber > 0) {
			fprintf(stdout, "Number of Temperature Sensors: %i\n", t->GetTempSensors());
			t->SelectSensor(0);
			fprintf(stdout, "Temperature: %.2f C\n", (float)t->GetTemp()/256);
		}
		if (fanNumber > 0) {
			for(i = 0 ; i< 5; i++) {
				f->ReadFanInfo(i, &f->fanInfo[i]);
			}
			for(i = 0 ; i< 5; i++) {
				f->PrintInfo(f->fanInfo[i]);
			}
		}
	}
	else {
		if(fanNumber != 0) {
			if(fanMode != 0 || fanRPM != 0) {
				if(fanMode == FixedRPM && fanRPM <= 0) {
					fprintf(stderr, "Fan RMP missing for Fixed RPM fan mode.\n");
					return 1;
				}
				else {
					CorsairFan::CorsairFanInfo newFanInfo;
					//fan->SelectFan(fanNumber - 1);
					if(fanMode != 0) {
						fprintf(stdout, "Setting fan to mode %s\n", CorsairFan::GetFanModeString(fanMode));
						newFanInfo.Mode = fanMode;
					}
					if(fanRPM != 0) {
						fprintf(stdout, "Setting fan RPM to %i\n", fanRPM);
						newFanInfo.RPM = fanRPM;
					}
					f->SetFansInfo(fanNumber - 1, newFanInfo);
				}
			} else {
				fprintf(stdout, "No mode or fan RPM specified for the fan.\n");
				return 1;
			}
		}
		else if(fanMode != 0 || fanRPM != 0) {
				fprintf(stderr, "Cannot set fan to a specific mode or fixed RPM without specifying the fan number\n");
				return 1;
		}

		if(ledNumber != 0) {
			l->SelectLed(ledNumber - 1);
			fprintf(stdout, "Setting LED\n");
			if (ledMode > -1) {
				l->SetMode(ledMode);
			}
			int current_mode = l->GetMode();
			//fprintf(stdout, "DEBUG Current LED Mode: %02x\n", current_mode);
			if ((current_mode == 0x0C) && (l->color_set_by_opts == 3)) {
				l->Set_TempMode_Color(l->color);
			}
			if ((current_mode != 0x0C)&&(l->color_set_by_opts > 0)) {
				//fprintf(stdout, "DEBUG Setting Led Colorset; colors defined: %i\n", l->color_set_by_opts);
				l->SetLedCycleColors(l->color);
			}
		}
	}

	if(cl != NULL) {
		delete cl;
		delete f;
		delete l;
		delete t;
	}
	return 0;
} 
