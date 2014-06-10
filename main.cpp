#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include "CorsairLink.h"

using namespace std;

static struct option long_options[] = {
	{"help",  no_argument, 0, 'h'},
	{"fan", required_argument, 0, 'f'},
	{"mode", required_argument, 0, 'm'},
	{"rpm",  required_argument, 0, 'r'},
	{0, 0, 0, 0}
};

int parseArguments(int argc, char **argv, int &fanNumber, int &fanMode, int &fanRPM);

int main(int argc, char **argv) {
	fprintf(stdout, "Open Corsair Link\n");
	int fanNumber = 0, fanMode = 0, fanRPM = 0;
	if(parseArguments(argc, argv, fanNumber, fanMode, fanRPM)) {
		return 1;
	}
	CorsairLink *cl = new CorsairLink();

	if(!cl->Initialize()) {
		fprintf(stdout, "Cannot initialize link.");
		delete cl;
		return 1;
	}

	if(fanNumber != 0) {
		if(fanMode != 0 || fanRPM != 0) {
			if(fanMode == FixedRPM && fanRPM <= 0) {
				fprintf(stderr, "Fan RMP missing for Fixed RPM fan mode.");
				return 1;
			}
			else {
				CorsairFan fanInfo;
				if(fanMode != 0) {
					fprintf(stdout, "Setting fan to mode %s\n", CorsairFanInfo::GetFanModeString(fanMode));
					fanInfo.Mode = fanMode;
				}
				if(fanRPM != 0) {
					fprintf(stdout, "Setting fan RPM to %i\n", fanRPM);
					fanInfo.RPM = fanRPM;
				}
				cl->SetFansInfo(fanNumber - 1, fanInfo);
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
	else {
		int i = 0;

		cl->ReadFansInfo();
		//std::cout << "Number of fans: " << (sizeof(cl->fans)/sizeof(*cl->fans)) << endl;
		for(i = 0 ; i< 5; i++) {
			cl->fans[i].PrintInfo();
		}
	}

	if(cl != NULL) {
		delete cl;
	}
	return 0;
} 

void printHelp() {
	fprintf(stdout, "OpenCorsairLink [options]\n");
	fprintf(stdout, "Options:\n");
	fprintf(stdout, "\t-f, --fan <fan number> Selects a fan to setup.\n");
	fprintf(stdout, "\t\tAccepted values are 1, 2, 3 or 4.\n");
	fprintf(stdout, "\t\t5 is H100i Pump.\n");
	fprintf(stdout, "\t-m, --mode <mode> Sets the mode for the selected fan");
	fprintf(stdout, "\t\tModes:");
	fprintf(stdout, "\t\t\t 4 - Fixed RPM (requires to specify the RPM)\n");
	fprintf(stdout, "\t\t\t 6 - Default\n");
	fprintf(stdout, "\t\t\t 8 - Quiet\n");
	fprintf(stdout, "\t\t\t10 - Balanced\n");
	fprintf(stdout, "\t\t\t12 - Performance\n");
	fprintf(stdout, "\t-r, --rpm <fan RPM> The desired RPM for the selected fan. NOTE: it works only when fan mode is set to Fixed RPM\n");
	fprintf(stdout, "\t-h, --help Prints this message\n");
	fprintf(stdout, "\nNot specifying any option will display information about the fans and pump\n\n");
}

int parseArguments(int argc, char **argv, int &fanNumber, int &fanMode, int &fanRPM) {
	int c, returnCode = 0;;
	while (1) {
		int option_index = 0;

		c = getopt_long (argc, argv, "f:m:r:h", long_options, &option_index);
		if (c == -1 || returnCode != 0)
			break;
		switch (c) {
		case 'f':
			errno = 0;
			fanNumber = strtol(optarg, NULL, 10);
			if(fanNumber < 1 || fanNumber > 5){
				fprintf(stderr, "Fan number is invalid. Accepted values are 1, 2, 3 or 4.");
				returnCode = 1;
			}
			break;

		case 'm':
			errno = 0;
			fanMode = strtol(optarg, NULL, 10);
			if(fanMode != Performance && fanMode != FixedRPM &&
				fanMode != Default && fanMode != Balanced && fanMode != Quiet){

				fprintf(stderr, "Fan mode is not allowed. Accepted values are:\n");
				fprintf(stderr, "\t 4 - Fixed RPM\n");
				fprintf(stderr, "\t 6 - Default\n");
				fprintf(stderr, "\t 8 - Quiet\n");
				fprintf(stderr, "\t10 - Balanced\n");
				fprintf(stderr, "\t12 - Performance\n");
				returnCode = 1;
			}
			break;

		case 'r':
			errno = 0;
			fanRPM = strtol(optarg, NULL, 10);
			if(fanRPM < 0){
				fprintf(stderr, "Fan RPM cannot be a negative value.\n");
				returnCode = 1;
			}
			break;

		case 'h':
			printHelp();
			exit(0);
			break;

		default:
			exit(1);
			returnCode = 0;
		}
	}
	return returnCode;
}
