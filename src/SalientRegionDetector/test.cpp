/*
 * test.h
 *
 *  Created on: 7 May 2014
 *      Author: pihatonttu
 */

#include <iostream>
#include <string>
#include "SalientRegionDetector.h"

using namespace std;

int main(int argc, char* argv[])
{

	if ((argc == 2 && strncmp(argv[1], "-s", 2) == 0) || argc == 1) {
		std::cerr << "Usage: " << argv[0] << " [-s] img1 img2..." << std::endl;
		        return 1;
	}
	vector<string> picvec(0);
	bool doSegmentation = strncmp(argv[1], "-s", 2) == 0 ? true : false;
	int fimg = doSegmentation ? 2 : 1;
	for (int i = fimg; i < argc; i++) {
		string imgName = argv[i];
		picvec.push_back(imgName);
	}

	SalientRegionDetector SDR;
	SDR.DetectSaliency(picvec, doSegmentation);
}
