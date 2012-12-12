# -------------------------------------------------
# Project created by QtCreator 2012-12-08T23:05:59
# -------------------------------------------------
TARGET = openCV-GUI
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
#    MainWidget.cpp \
    ImageCapture.cpp
HEADERS += mainwindow.h \
 #   MainWidget.h \
    ImageCapture.h
FORMS += mainwindow.ui
#FORMS += src/ui/MainWidget.ui

# win32 {
# CV11_INCLUDE = C:/OpenCV
# CV11_LIB = C:/OpenCV/lib
# CV22_INCLUDE = C:/OpenCV2.2/modules
# CV22_LIB = C:/OpenCV2.2/build/bin
# } else{
# CV_INCLUDE = /usr/local/include/opencv2
# CV_LIB = /usr/local/lib
# }
# message(build with opencv 2.2)
# DEFINES += OPEN_CV_22
# INCLUDEPATH += $$CV_INCLUDE/core \
# $$CV_INCLUDE/highgui \
# $$CV_INCLUDE/imgproc
# LIBS += -L$$CV_LIB
# LIBS += -lopencv_highgui -lopencv_core -lopencv_imgproc
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
