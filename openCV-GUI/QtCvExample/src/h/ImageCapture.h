#ifndef _IMAGE_CAPTURE_H_
#define _IMAGE_CAPTURE_H_

#include <QObject>
#include <QPixmap>

extern "C"{
	
#ifdef OPEN_CV_22
	// in order to make it work with old version of OpenCV,
	// we use C-style interface from OpenCV 2.2
//        #include <core.hpp>
        #include <core_c.h>
	#include <imgproc_c.h>
	#include <highgui_c.h>
#else
	#include <cv.h>
	#include <highgui.h>
#endif

}

class ImageCapture : public QObject{
	Q_OBJECT
public:
	ImageCapture( QObject * parent = NULL );
	~ImageCapture();
public slots:
	void captureFromFile( const QString& path );//!< attempts to load video file
	void captureFromCamera( int cameraIndex = -1 );	//!< attempts to start camera, camera number is given in cameraIndex (see cvCaptureFromCAM docs)
	void enableProcessing( bool on );			//!< switch edge detection on / off	
	void flipImage( bool on );					//!< switch image flipping on / off
	void setEdgeDetectorThreshold( int value );	//!< set value for edge detection algorithm
	void stopCapture();							//!< stops timer and release OpenCV capture structure
protected:
	QImage convert( IplImage * image );			//!< create QImage from OpenCV image data
protected slots:
	void timerTick();							//!< grab and send frame on each timer tick
signals:
	void imageCaptured( const QImage& image );
	void error( const QString& text );
protected:
	QTimer * _timer;	//!< timeout signal is used to capture frames at regular intervals (or whenever possible in case of camera)
//        Mat * _cvCap;	//!< used to grab image data from video or camera
        CvCapture * _cvCap;	//!< used to grab image data from video or camera

	bool _processImage;	//!< if true, then we run edge detection on image
	int _threshold;		//!< parameter for edge detection algorithm
	bool _flipImage;	//!< if true, then final image is flipped (horizontal)
};

#endif
