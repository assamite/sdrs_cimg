
all: sdrs

sdrs: meanshift CImgHandler.o
	g++ -o saliency Saliency.cpp SalientRegionDetector.cpp ms.o rlist.o RAList.o msImageProcessor.o CImgHandler.o  -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

meanshift: MeanShiftCode/ms.cpp MeanShiftCode/msImageProcessor.cpp MeanShiftCode/rlist.cpp MeanShiftCode/RAList.cpp
	g++ -c MeanShiftCode/ms.cpp MeanShiftCode/msImageProcessor.cpp MeanShiftCode/rlist.cpp MeanShiftCode/RAList.cpp -O2

CImgHandler.o:  CImgHandler.cpp
	 g++ -c CimgHandler.cpp -O2 -I/usr/X11R6/include -I/opt/local/include 

clean:
	rm -rf *.o

shared_osx: meanshift CImgHandler.o
	g++ -dynamiclib -Wall -I/usr/include/python2.7/ -lpython2.7 -lz -o saliency.dylib Saliency.cpp SalientRegionDetector.cpp ms.o rlist.o RAList.o msImageProcessor.o CImgHandler.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -lpng -ljpeg
	mv ./saliency.dylib ./saliency.so
	
shared_linux: meanshift CImgHandler.o
	g++ -shared -Wall -g -fPIC -I/usr/include -I/usr/lib -I/usr/include/python2.7/ -lpython2.7 -o saliency.so Saliency.cpp SalientRegionDetector.cpp ms.o rlist.o RAList.o msImageProcessor.o CImgHandler.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
	
