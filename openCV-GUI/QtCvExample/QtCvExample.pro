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
} else {
        CV11_INCLUDE = /usr/local/include/opencv
        CV11_LIB = /usr/local/lib
        CV22_INCLUDE = /usr/local/include/OpenCV2.2
        CV22_LIB = /usr/local/lib/OpenCV2.2
        CV_INCLUDE = /usr/local/include/opencv2
        CV_LIB = /usr/local/lib
}

CONFIG += cv3

cv1 {
        message(build with opencv 2.2)
        DEFINES += OPEN_CV_22
        INCLUDEPATH += $$CV22_INCLUDE/core/include/opencv2/core \
                                   $$CV22_INCLUDE/highgui/include/opencv2/highgui \
                                   $$CV22_INCLUDE/imgproc/include/opencv2/imgproc \
                                   $$CV22_INCLUDE/core/include	\
                                   $$CV22_INCLUDE/imgproc/include
        LIBS += -L$$CV22_LIB
        LIBS += -lopencv_highgui220 -lopencv_core220 -lopencv_imgproc220
}

cv2 {
        message(build with opencv 1.1)
        DEFINES -= OPEN_CV_22
        INCLUDEPATH += $$CV11_INCLUDE
        LIBS += -L$$CV11_LIB
        #LIBS += -lopencv -lhighgui -lcxcore
#        LIBS += -lopencv_highgui -lopencv_core -lopencv_imgproc
LIBS += -lopencv_calib3d \
-lopencv_contrib \
-lopencv_core \
-lopencv_features2d \
-lopencv_flann \
-lopencv_gpu \
-lopencv_highgui \
-lopencv_imgproc \
-lopencv_legacy \
-lopencv_ml \
-lopencv_nonfree \
-lopencv_objdetect \
-lopencv_photo \
-lopencv_stitching \
-lopencv_ts \
-lopencv_video \
-lopencv_videostab
}

cv3 {
message(build with custom)
DEFINES += OPEN_CV_22
INCLUDEPATH += $$CV_INCLUDE/core \
               $$CV_INCLUDE/highgui \
               $$CV_INCLUDE/imgproc
LIBS += -L$$CV_LIB
LIBS += -lopencv_highgui -lopencv_core -lopencv_imgproc
}


#win32 {
#	CV11_INCLUDE = C:/OpenCV
#	CV11_LIB = C:/OpenCV/lib
#	CV22_INCLUDE = C:/OpenCV2.2/modules
#	CV22_LIB = C:/OpenCV2.2/build/bin
#} else {
#	CV_INCLUDE = /usr/local/include/opencv2
#	CV_LIB = /usr/local/lib
#}

#message(build with opencv 2.2)
#DEFINES += OPEN_CV_22
#INCLUDEPATH += $$CV_INCLUDE/core \
#               $$CV_INCLUDE/highgui \
#	       $$CV_INCLUDE/imgproc
#LIBS += -L$$CV_LIB
#LIBS += -lopencv_highgui -lopencv_core -lopencv_imgproc

HEADERS += src/h/ImageCapture.h src/h/MainWidget.h
FORMS += src/ui/MainWidget.ui
SOURCES += src/cpp/ImageCapture.cpp src/cpp/MainWidget.cpp main.cpp
