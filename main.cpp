#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include "CorsairLink.h"

using namespace std;

static struct option long_options[] =
	             {
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
				CorsairFanInfo fanInfo;
				if(fanMode != 0) {
					std::cout << "Setting fan to mode " << CorsairFanInfo::GetFanModeString(fanMode) << endl;
					fanInfo.Mode = fanMode;
				}
				if(fanRPM != 0) {
					std::cout << "Setting fan RPM to " << fanRPM << endl;
					fanInfo.RPM = fanRPM;
				}
				cl->SetFansInfo(fanNumber - 1, fanInfo);
			}
		} else {
			std::cout << "No mode or fan RPM specified for the fan." << endl;
			return 1;
		}
	}
	else if(fanMode != 0 || fanRPM != 0) {
			std::cerr<< "Cannot set fan to a specific mode or fixed RPM without specifying the fan number" << endl;
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
	std::cout << "OpenCorsairLink [options]" << endl;
	std::cout << "Options:" << endl;
	std::cout << "\t-f, --fan <fan number> Selects a fan to setup. Accepted values are 1, 2, 3 or 4. 5 is H100i Pump." << endl;
	std::cout << "\t-m, --mode <mode> Sets the mode for the selected fan" << endl;
	std::cout << "\t                  Modes:" << endl;
	std::cout << "\t                     4 - Fixed RPM (requires to specify the RPM)" << endl;
	std::cout << "\t                     6 - Default" << endl;
	std::cout << "\t                     8 - Quiet" << endl;
	std::cout << "\t                    10 - Balanced" << endl;
	std::cout << "\t                    12 - Performance" << endl;
	std::cout << "\t-r, --rpm <fan RPM> The desired RPM for the selected fan. NOTE: it works only when fan mode is set to Fixed RPM" << endl;
	std::cout << "\t-h, --help Prints this message" << endl;
	std::cout << endl << "Not specifying any option will display information about the fans and pump" << endl << endl;
}

int parseArguments(int argc, char **argv, int &fanNumber, int &fanMode, int &fanRPM) {
	int c, returnCode = 0;;
	while (1) {
		int option_index = 0;

		c = getopt_long (argc, argv, "f:m:r:h", long_options, &option_index);
		//std::cout << c;
		if (c == -1 || returnCode != 0)
			break;
		switch (c) {
		case 'f':
			errno = 0;
			fanNumber = strtol(optarg, NULL, 10);
			if(fanNumber < 1 || fanNumber > 5){
				std::cerr << "Fan number is invalid. Accepted values are 1, 2, 3, 4 or 5." << endl;
				returnCode = 1;
			}
			break;

		case 'm':
			errno = 0;
			fanMode = strtol(optarg, NULL, 10);
			if(fanMode != Performance && fanMode != FixedRPM &&
				fanMode != Default && fanMode != Balanced && fanMode != Quiet){

				std::cerr << "Fan mode is not allowed. Accepted values are:" << endl
					<< "\t 4 - Fixed RPM"<< endl
					<< "\t 6 - Default" << endl
					<< "\t 8 - Quiet" << endl
					<< "\t10 - Balanced" << endl
					<< "\t12 - Performance" << endl;
				returnCode = 1;
			}
			break;

		case 'r':
			errno = 0;
			fanRPM = strtol(optarg, NULL, 10);
			if(fanRPM < 0){
				std::cerr << "Fan RPM cannot be a negative value." << endl;
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
