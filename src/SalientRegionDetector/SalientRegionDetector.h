/*
 * SalienRegionDetector.h
 *
 *  Created on: 7 May 2014
 *      Author: pihatonttu
 */

#ifndef SALIENREGIONDETECTOR_H_
#define SALIENREGIONDETECTOR_H_

#include <vector>
#include <string>
using namespace std;

#pragma once

#ifndef UINT
#define UINT unsigned int
#endif

class SalientRegionDetector
{

public:
	SalientRegionDetector();
	virtual ~SalientRegionDetector();

	void DetectSaliency(
		vector<string>&							picvec,
		bool&									doSegmentation);


private:

	void DoMeanShiftSegmentation(
		const vector<UINT>&						inputImg,
		const int&								width,
		const int&								height,
		vector<UINT>&							segimg,
		const int&								sigmaS,
		const float&							sigmaR,
		const int&								minRegion,
		vector<int>&							labels,
		int&									numlabels);

	void DrawContoursAroundSegments(
		vector<UINT>&							segmentedImage,
		const int&								width,
		const int&								height,
		const UINT&								color);

	void ChooseSalientPixelsToShow(
		const vector<double>&					salmap,
		const int&								width,
		const int&								height,
		const vector<int>&						labels,
		const int&								numlabels,
		vector<bool>&							choose);

	void DoMeanShiftSegmentationBasedProcessing(
		const vector<UINT>&						inputImg,
		const int&								width,
		const int&								height,
		const string&							filename,
		const vector<double>&					salmap,
		const int&								sigmaS,
		const float&							sigmaR,
		const int&								minRegion,
		vector<UINT>&							segimg,
		vector<UINT>&							segobj);

};

#endif /* SALIENREGIONDETECTOR_H_ */
