/*
 * SalientRegionDetector.cpp
 */

#include <iostream>
#include "SalientRegionDetector.h"
#include "CImgHandler.h"
#include "Saliency.h"
#include "MeanShiftCode/msImageProcessor.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//===========================================================================
///	Construction/Destruction
//===========================================================================
SalientRegionDetector::SalientRegionDetector()
{

}

SalientRegionDetector::~SalientRegionDetector()
{

}


//===========================================================================
///	DoMeanShiftSegmentation
//===========================================================================
void SalientRegionDetector::DoMeanShiftSegmentation(
	const vector<UINT>&						inputImg,
	const int&								width,
	const int&								height,
	vector<UINT>&							segimg,
	const int&								sigmaS,
	const float&							sigmaR,
	const int&								minRegion,
	vector<int>&							labels,
	int&									numlabels)
{
	int sz = width*height;
	byte* bytebuff = new byte[sz*3];
	{int i(0);
	for( int p = 0; p < sz; p++ )
	{
		bytebuff[i+0] = inputImg[p] >> 16 & 0xff;
		bytebuff[i+1] = inputImg[p] >>  8 & 0xff;
		bytebuff[i+2] = inputImg[p]       & 0xff;
		i += 3;
	}}
	msImageProcessor mss;
	mss.DefineImage(bytebuff, COLOR, height, width);
	mss.Segment(sigmaS, sigmaR, minRegion, HIGH_SPEEDUP);
	mss.GetResults(bytebuff);

	int* p_labels = new int[sz];
	numlabels = mss.GetLabels(p_labels);
	labels.resize(sz);
	for( int n = 0; n < sz; n++ ) labels[n] = p_labels[n];
	if(p_labels) delete [] p_labels;

	segimg.resize(sz);
	int bsz = sz*3;
	{int i(0);
	for( int p = 0; p < bsz; p += 3 )
	{
		segimg[i] = bytebuff[p] << 16 | bytebuff[p+1] << 8 | bytebuff[p+2];
		i++;
	}}
	if(bytebuff) delete [] bytebuff;
}

//=================================================================================
/// DrawContoursAroundSegments
//=================================================================================
void SalientRegionDetector::DrawContoursAroundSegments(
	vector<UINT>&							segmentedImage,
	const int&								width,
	const int&								height,
	const UINT&								color)
{
	// Pixel offsets around the centre pixels starting from left, going clockwise
	const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};

	int sz = segmentedImage.size();
	vector<bool> istaken(sz, false);
	int mainindex(0);
	for( int j = 0; j < height; j++ )
	{
		for( int k = 0; k < width; k++ )
		{
			int np(0);
			for( int i = 0; i < 8; i++ )
			{
				int x = k + dx8[i];
				int y = j + dy8[i];

				if( (x >= 0 && x < width) && (y >= 0 && y < height) )
				{
					int index = y*width + x;
					if( false == istaken[index] )
					{
						if( (int)segmentedImage[mainindex] != (int)segmentedImage[index] ) np++;
					}
				}
			}
			if( np > 2 )//1 for thicker lines and 2 for thinner lines
			{
				segmentedImage[j*width + k] = color;
				istaken[mainindex] = true;
			}
			mainindex++;
		}
	}
}


//=================================================================================
// ChooseSalientPixelsToShow
//=================================================================================
void SalientRegionDetector::ChooseSalientPixelsToShow(
	const vector<double>&					salmap,
	const int&								width,
	const int&								height,
	const vector<int>&						labels,
	const int&								numlabels,
	vector<bool>&							choose)
{
	int sz = width*height;
	//----------------------------------
	// Find average saliency per segment
	//----------------------------------
	vector<double> salperseg(numlabels,0);
	vector<int> segsz(numlabels,0);
	vector<bool> touchborders(numlabels, false);
	{int i(0);
	for( int j = 0; j < height; j++ )
	{
		for( int k = 0; k < width; k++ )
		{
			salperseg[labels[i]] += salmap[i];
			segsz[labels[i]]++;

			if(false == touchborders[labels[i]] && (j == height-1 || j == 0 || k == width-1 || k == 0) )
			{
				touchborders[labels[i]] = true;
			}
			i++;
		}
	}}

	double avgimgsal(0);
	{for( int n = 0; n < numlabels; n++ )
	{
		if(true == touchborders[n])
		{
			salperseg[n] = 0;
		}
		else
		{
			avgimgsal += salperseg[n];
			salperseg[n] /= segsz[n];
		}
	}}

	//--------------------------------------
	// Compute average saliency of the image
	//--------------------------------------
	avgimgsal /= sz;


	//----------------------------------------------------------------------------
	// Choose segments that have average saliency twice the average image saliency
	//----------------------------------------------------------------------------
	vector<bool> segtochoose(numlabels, false);
	{for( int n = 0; n < numlabels; n++ )
	{
		if( salperseg[n] > (avgimgsal+avgimgsal) ) segtochoose[n] = true;
	}}

	choose.resize(sz, false);
	bool atleastonesegmentchosent(false);
	{for( int s = 0; s < sz; s++ )
	{
		//if( salperseg[labels[s]] > (avgsal+avgsal) )
		//if(true == segtochoose[labels[s]])
		{
			choose[s] = segtochoose[labels[s]];
			atleastonesegmentchosent = choose[s];
		}
	}}

	//----------------------------------------------------------------------------
	// If not a single segment has been chosen, then take the brightest one available
	//----------------------------------------------------------------------------
	if( false == atleastonesegmentchosent )
	{
		int maxsalindex(-1);
		double maxsal(DBL_MIN);
		for( int n = 0; n < numlabels; n++ )
		{
			if( maxsal < salperseg[n] )
			{
				maxsal = salperseg[n];
				maxsalindex = n;
			}
		}
		for( int s = 0; s < sz; s++ )
		{
			if(maxsalindex == labels[s]) choose[s] = true;
		}
	}
}

//===========================================================================
///	DoMeanShiftSegmentationBasedProcessing
///
///	Do the segmentation of salient region based on K-Means segmentation
//===========================================================================
void SalientRegionDetector::DoMeanShiftSegmentationBasedProcessing(
	const vector<UINT>&						inputImg,
	const int&								width,
	const int&								height,
	const string&							filename,
	const vector<double>&					salmap,
	const int&								sigmaS,
	const float&							sigmaR,
	const int&								minRegion,
	vector<UINT>&							segimg,
	vector<UINT>&							segobj)
{
	int sz = width*height;
	//--------------------------------------------------------------------
	// Segment the image using mean-shift algo. Segmented image in segimg.
	//--------------------------------------------------------------------
	vector<int> labels(0);
	int numlabels(0);
	DoMeanShiftSegmentation(inputImg, width, height, segimg, sigmaS, sigmaR, minRegion, labels, numlabels);
	//-----------------
	// Choose segments
	//-----------------
	vector<bool> choose(0);
	ChooseSalientPixelsToShow(salmap, width, height, labels, numlabels, choose);
	//-----------------------------------------------------------------------------
	// Take up only those pixels that are allowed by finalPixelMap
	//-----------------------------------------------------------------------------
	segobj.resize(sz, 0);
	for( int p = 0; p < sz; p++ )
	{
		if( choose[p] )
		{
			segobj[p] = inputImg[p];
		}
	}
}


/**
 * Crux of the program.
 *
 *
 */
void SalientRegionDetector::DetectSaliency(
		string&						outfolder,
		vector<string>&				picvec,
		bool&						doSegmentation)
{
	CImgHandler cimgHand;
	int numPics(picvec.size());

	for(int k = 0; k < numPics; k++)
	{
		vector<UINT> img(0);
		int width(0);
		int height(0);

		cimgHand.GetPictureBuffer(picvec[k], img, width, height);
		int sz = width*height;

		Saliency sal;
		vector<double> salmap(0);
		sal.GetSaliencyMap(img, width, height, salmap, true);

		vector<UINT> outimg(sz);
		for( int i = 0; i < sz; i++ )
		{
			int val = salmap[i] + 0.5;
			outimg[i] = val << 16 | val << 8 | val;
		}
		string suffix = "_salmap.jpg";
		cimgHand.SavePicture(outimg, width, height, outfolder, picvec[k], suffix);

		if(doSegmentation)
		{
			vector<UINT> segimg, segobj;
			DoMeanShiftSegmentationBasedProcessing(img, width, height, picvec[k], salmap, 7,10,20, segimg, segobj);
			DrawContoursAroundSegments(segimg, width, height, 0xffffff);
			//DrawContoursAroundSegments(segobj, width, height, 0xffffff);
			suffix = "_meanshift.jpg";
			cimgHand.SavePicture(segimg, width, height, outfolder, picvec[k], suffix);
			suffix = "_salientobject.jpg";
			cimgHand.SavePicture(segobj, width, height, outfolder, picvec[k], suffix);
		}
	}
}

extern "C" {
	/**
	 * C style functions for python wrapper. Accepts only one picture.
	 */
	SalientRegionDetector* SalientRegionDetector_New()
	{
		return new SalientRegionDetector();
	}

	int saliency(SalientRegionDetector* sdr, char* outfolder, char* pic, bool doSegmentation)
	{
		try
		{
			vector<string> picvec(0);
			picvec.push_back(pic);
			string of = outfolder;
			sdr->DetectSaliency(of, picvec, doSegmentation);
		}
		catch (int e)
		{
			return -1;
		}
		return 0;
	}
}

void printUsage(char * argv1)
{
	std::cerr << "This program implements the saliency detection and segmentation method described in:" << std::endl;
	std::cerr << "R. Achanta, S. Hemami, F. Estrada and S. S�sstrunk, Frequency-tuned Salient Region Detection," << std::endl;
	std::cerr << "IEEE International Conference on Computer Vision and Pattern Recognition (CVPR), 2009" << std::endl;
	std::cerr << std::endl;
	std::cerr << "Usage: " << argv1 << " [-s] [-o=path/to/outfolder/] img1 img2..." << std::endl;
	std::cerr << std::endl;
	std::cerr << "By default only saliency map of the image is extracted. Mean shift based processing and most" << std::endl;
	std::cerr << "salient object exraction can be enabled by using '-s' flag at the start of the arguments." << std::endl;
	std::cerr << std::endl;
}

int main(int argc, char* argv[])
{
	bool doSegmentation = false;
	string outFolder = "";
	vector<string> picvec(0);
	if (argc == 1)
	{
		printUsage(argv[0]);
		return 1;
	}
	for (int i = 1; i < argc; i++)
	{
		if (strncmp(argv[i], "-s", 2) == 0)
		{
			doSegmentation = true;
		}
		else if (strncmp(argv[i], "-o=", 3) == 0)
		{
			string a = argv[i];
			outFolder = a.substr(3, a.length() -3);
			std::cout << outFolder << std::endl;
		}
		else
		{
			picvec.push_back(argv[i]);
		}
	}
	if (picvec.size() == 0)
	{
		printUsage(argv[0]);
		return 1;
	}
	SalientRegionDetector SDR;
	SDR.DetectSaliency(outFolder, picvec, doSegmentation);
}



