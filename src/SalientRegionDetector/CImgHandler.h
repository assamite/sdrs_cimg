/**
 * CImgHandler.h
 */
#include <vector>
#include <algorithm>

#if !defined(CIMGHANDLER_INCLUDED_)
#define CIMGHANDLER_INCLUDED_

#if !defined(UINT)
#define UINT uint32_t
#endif

using namespace std;

/**
 * Image save and load back end using CImg.
 */
class CImgHandler
{
public:
	CImgHandler();
	virtual ~CImgHandler();

	void							GetPictureBuffer(
										string&				filename,
										vector<UINT>&		outBuff,
										int&				width,
										int&				height);

	void							SavePicture(
										vector<UINT>&		imgBuffer,
										int					width,
										int					height,
										string&				outFolder,
										string&				outFilename,
										string&				suffix);

private:

};

#endif // !defined(PICTUREHANDLER_INCLUDED_)

