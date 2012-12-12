TEMPLATE = app
DESTDIR = bin
TARGET = cvexample
DEPENDPATH += . src/cpp src/h src/ui
INCLUDEPATH += . src/h

UI_DIR = build
MOC_DIR = build

build_pass:CONFIG(debug, debug|release){
	OBJECTS_DIR = build/debug
} else{
	OBJECTS_DIR = build/release
}

win32 {
	CV11_INCLUDE = C:/OpenCV
	CV11_LIB = C:/OpenCV/lib
	CV22_INCLUDE = C:/OpenCV2.2/modules
	CV22_LIB = C:/OpenCV2.2/build/bin
} else{
	CV11_INCLUDE = /usr/local/include/OpenCV
	CV11_LIB = /usr/local/lib/OpenCV
	CV22_INCLUDE = /usr/local/include/OpenCV2.2
	CV22_LIB = /usr/local/lib/OpenCV2.2
}

contains(cv,2){
	message(build with opencv 2.2)
	DEFINES += OPEN_CV_22
	INCLUDEPATH += $$CV22_INCLUDE/core/include/opencv2/core \
				   $$CV22_INCLUDE/highgui/include/opencv2/highgui \
				   $$CV22_INCLUDE/imgproc/include/opencv2/imgproc \
				   $$CV22_INCLUDE/core/include	\
				   $$CV22_INCLUDE/imgproc/include
	LIBS += -L$$CV22_LIB
	LIBS += -lopencv_highgui220 -lopencv_core220 -lopencv_imgproc220
} else{
	message(build with opencv 1.1)
	DEFINES -= OPEN_CV_22
	INCLUDEPATH += $$CV11_INCLUDE/cv/include $$CV11_INCLUDE/otherlibs/highgui $$CV11_INCLUDE/cxcore/include
	LIBS += -L$$CV11_LIB
	LIBS += -lcv -lhighgui -lcxcore
}

HEADERS += src/h/ImageCapture.h src/h/MainWidget.h
FORMS += src/ui/MainWidget.ui
SOURCES += src/cpp/ImageCapture.cpp src/cpp/MainWidget.cpp main.cpp
