#include <vector>
#include <algorithm>

#if !defined(CIMGHANDLER_INCLUDED_)
#define CIMGHANDLER_INCLUDED_

#if !defined(UINT)
#define UINT unsigned int
#endif

using namespace std;

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
										string&				outFilename,
										string&				suffix);

private:

};

#endif // !defined(PICTUREHANDLER_INCLUDED_)

