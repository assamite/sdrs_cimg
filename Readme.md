Salient Region Detection and Segmentation for Python Using CImg
======================================================

This is a platform independent (not tested in Windows) version of Salient Region Detection and Segmentation technique represented in:

[R. Achanta, S. Hemami, F. Estrada and S. Süsstrunk, Frequency-tuned Salient Region Detection, IEEE International Conference on Computer Vision and Pattern Recognition (CVPR 2009), pp. 1597 - 1604, 2009.](http://infoscience.epfl.ch/record/135217/files/1708.pdf)

Check also the [webpage](http://ivrgwww.epfl.ch/supplementary_material/RK_CVPR09/index.html) for the paper. 
Most of the code is unaltered from the [C++ sources](http://ivrgwww.epfl.ch/supplementary_material/RK_CVPR09/SourceCode/SalientRegionDetectorAndSegmenter.zip) given on the webpage. 

The differences are: 
* Use of [CImg](http://cimg.sourceforge.net/) to handle picture loading and saving for platform independence
* Changed the code to work as a command line tool
* Added Python wrapper for main functionality with ctypes

Dependencies: CImg, boost, (libpng, libjpeg...)

*The conversion has been done with minimum amount of work so any input and/or change suggestions are welcome.*

Usage
------------------------------------------------------

1. Download CImg and put `CImg.h` somewhere your compiler finds it

2. Download boost and configure it. Boost is only used for filepath manipulation. In case you don't want to use boost, you can alter the source of `CImgHandler::SavePictures`.

3. As Command line tool:

	3.1 Compile program with `make` (and `make clean`)

	3.2 Call the program created:

	`$>./saliency [-s] [-o=path/to/outfolder/] path/to/pic1 path/to/pic2 path/to/pic3 ...`

	where optional '-s' argument tells the program to not only do saliency mapping, but also mean shift based processing and extracting the most salient objects from the pictures.
	
4. From Python:

	4.1 Compile program as shared library with `make shared_osx` or `make shared_linux` (and `make clean`)
	
	4.2 Instantiate `SDRWrapper` object in Python and call it:
	
		from sdrWrapper import SDRWrapper
		sdrw = SDRWrapper()
		doSegmentation = True
		sdrw.saliency("path/to/outfolder", "path/to/my/pic", doSegmentation)

