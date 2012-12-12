#-------------------------------------------------
#
# Project created by QtCreator 2012-12-09T02:11:59
#
#-------------------------------------------------

QT       -= gui

TARGET = openCV-integration
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

CV11_INCLUDE = /usr/local/include/opencv
CV11_LIB = /usr/local/lib
CV22_INCLUDE = /usr/local/include/OpenCV2.2
CV22_LIB = /usr/local/lib/OpenCV2.2
CV_INCLUDE = /usr/local/include/opencv2
CV_LIB = /usr/local/lib

DEFINES -= OPEN_CV_22
INCLUDEPATH += $$CV11_INCLUDE
LIBS += -L$$CV11_LIB
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
