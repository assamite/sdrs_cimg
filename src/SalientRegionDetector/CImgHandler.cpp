/*
 * CImgHandler.cpp
 *
 *
 */

#include "CImg.h"
#include "CImgHandler.h"
#include "libgen.h"
#include <string>
#include <iostream>

using namespace cimg_library;

//=================================================================================
// Construction/Destruction
//=================================================================================

CImgHandler::CImgHandler()
{

}

CImgHandler::~CImgHandler()
{

}

//=================================================================================
//	GetPictureBuffer
//
//	Returns a buffer of the picture just opened
//=================================================================================
void CImgHandler::GetPictureBuffer(
	string&				filename,
	vector<UINT>&		imgBuffer,
	int&				width,
	int&				height)
{
	CImg<unsigned char> img(filename.c_str());
	height					= img.height();
	width					= img.width();
	long sz					= img.size();

	imgBuffer.resize(sz);
	int x = 0, y = 0, R = 0, G = 0, B = 0;
	for( int p = 0; p < sz; p++ ) {
		x = p % width;
		y = p / width;
		R = img(x, y, 0, 0);
		G = img(x, y, 0, 1);
		B = img(x, y, 0, 2);
		imgBuffer[p] = R << 16 | G << 8 | B;
	}
}


//=================================================================================
//	SavePicture
//
//	Saves the given buffer as a JPEG or BMP image depeding on which encoder CLSID
//	is used.
//=================================================================================
void CImgHandler::SavePicture(
	vector<UINT>&		img,
	int					width,
	int					height,
	string&				outFilename,
	string&				suffix)
{
	int sz = width*height;

	CImg<unsigned char> saveimg(width, height, 1, 3);
	int x = 0, y = 0, R = 0, G = 0, B = 0;
	for (int i = 0; i < sz; i++) {
		x = i % width;
		y = i / width;
		R = (img[i] >> 16) & 0xFF;
		G = (img[i] >>  8) & 0xFF;
		B = (img[i]      ) & 0xFF;
		saveimg(x, y, 0, 0) = R;
		saveimg(x, y, 0, 1) = G;
		saveimg(x, y, 0, 2) = B;
	}

	char* cstr = new char[outFilename.length() + 1];
	strcpy(cstr, outFilename.c_str());
	string bn = basename(cstr);
	int idx = bn.find_last_of(".");
	string name = bn.substr(0, idx);
	name += suffix;
	saveimg.save_jpeg(name.c_str(), 80);
}



