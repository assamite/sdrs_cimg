Salient Region Detection and Segmentation using CImg
======================================================

This is a platform independent (not tested in Windows) version of Salient Region Detection and Segmentation technique represented in:

[R. Achanta, S. Hemami, F. Estrada and S. Süsstrunk, Frequency-tuned Salient Region Detection, IEEE International Conference on Computer Vision and Pattern Recognition (CVPR 2009), pp. 1597 - 1604, 2009.](http://infoscience.epfl.ch/record/135217/files/1708.pdf)

Check also the [webpage](http://ivrgwww.epfl.ch/supplementary_material/RK_CVPR09/index.html) for the paper. 
Nearly all of the code is same as [C++ sources](http://ivrgwww.epfl.ch/supplementary_material/RK_CVPR09/SourceCode/SalientRegionDetectorAndSegmenter.zip) given on the webpage. 

The differences are: 
* Use of [CImg](http://cimg.sourceforge.net/) to handle picture loading and saving for platform independence
* Changing the code to work as a command line tool
* Adding Python wrapper for main functionality with ctypes

*The conversion has been done with minimum amount of work so any input and/or change suggestions are welcome.*

Usage
------------------------------------------------------

1. Download CImg and put CImg.h some where your compiler finds it

2. (optional) add additional image libraries (libpng, etc) to the Makefile's CImgHandler.o compile information so that CImg can load those types accordingly

3. As Command line tool:

	3.1 Run make (and make clean)

	3.2 Call the 'saliency' program created:

	`$> ./saliency [-s] path/to/pic1 path/to/pic2 path/to/pic3 ...`

	where optional '-s' argument tells the program to not only do saliency mapping, but also mean shift based processing and extracting the most salient objects from the pictures.
	
4. Inside Python:

	4.1 Run make shared_osx/shared_linux (and make clean)
	
	4.2 Instantiate SDRWrapper object:
	
		from sdrWrapper import SDRWrapper
		sdrw = SDRWrapper()
		doSegmentation = True
		sdrw.saliency("path/to/my/pic", doSegmentation)
	
	
The pictures are saved to the folder from where to code was run.
