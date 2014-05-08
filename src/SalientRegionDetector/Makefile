
all: sdrs

sdrs: cimg_handler.o meanshift
	g++ -o saliency Saliency.cpp SalientRegionDetector.cpp ms.o rlist.o RAList.o msImageProcessor.o cimg_handler.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

meanshift: MeanShiftCode/ms.cpp MeanShiftCode/msImageProcessor.cpp MeanShiftCode/rlist.cpp MeanShiftCode/RAList.cpp
	g++ -c MeanShiftCode/ms.cpp MeanShiftCode/msImageProcessor.cpp MeanShiftCode/rlist.cpp MeanShiftCode/RAList.cpp -O2

cimg_handler.o:  CImgHandler.cpp
	 g++ -c CimgHandler.cpp -o cimg_handler.o -O2 -I/usr/X11R6/include

clean:
	rm -rf *.o
