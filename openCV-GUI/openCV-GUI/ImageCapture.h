#ifndef IMAGECAPTURE_H
#define IMAGECAPTURE_H

#include <QObject>
#include <QImage>

#include <cv.h>
#include <highgui.h>

using namespace cv;

class ImageCapture : public QObject
{
Q_OBJECT
public:
    explicit ImageCapture(QObject *parent = 0);

signals:


public slots:
//	void captureFromFile( const QString& path );//!< attempts to load video file
//	void captureFromCamera( int cameraIndex = -1 );	//!< attempts to start camera, camera number is given in cameraIndex (see cvCaptureFromCAM docs)
        void enableProcessing( bool on );			//!< switch edge detection on / off
        void flipImage( bool on );					//!< switch image flipping on / off
        void setEdgeDetectorThreshold( int value );	//!< set value for edge detection algorithm
        void update();
//        void setCurrentImageMatrix(Mat M);
//        void loadFile(QString filename);
        void captureFromFile( const QString& path );//!< attempts to load video file

protected:
//	QTimer * _timer;	//!< timeout signal is used to capture frames at regular intervals (or whenever possible in case of camera)
//	CvCapture * _cvCap;	//!< used to grab image data from video or camera
        IplImage* current_IplImage;
        Mat current_image_matrix;
        bool _processImage;	//!< if true, then we run edge detection on image
        int _threshold;		//!< parameter for edge detection algorithm
        bool _flipImage;	//!< if true, then final image is flipped (horizontal)

public:
        QImage convert( IplImage * image );			//!< create QImage from OpenCV image data

signals:
        void error( const QString& text );
        void imageCaptured( const QImage& image );

};

#endif // IMAGECAPTURE_H
