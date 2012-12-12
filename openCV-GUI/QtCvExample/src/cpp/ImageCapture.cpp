#include "ImageCapture.h"
#include <QImage>
#include <QFile>
#include <QTimer>

ImageCapture::ImageCapture( QObject * parent )
	:QObject(parent)
	,_timer(NULL)
	,_cvCap(NULL)
	,_processImage(false)
	,_threshold(0)
	,_flipImage(false)
{
	this->_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(timerTick()));
}

ImageCapture::~ImageCapture(){
	this->stopCapture();
}

void ImageCapture::stopCapture(){
	this->_timer->stop();
	cvReleaseCapture(&_cvCap);
}

void ImageCapture::captureFromFile( const QString& path ){
	this->stopCapture();
	if( QFile::exists(path) ){
		this->_cvCap = cvCaptureFromAVI(path.toAscii().constData());
		if( this->_cvCap ){
			const double fps = cvGetCaptureProperty(this->_cvCap, CV_CAP_PROP_FPS);
			if( fps > 0.0 ){
				this->_timer->setInterval(1000.0/fps);
				this->_timer->start();
			} else{
				emit error(tr("Invalid fps for video: %1").arg(fps));
			}
		} else{
			emit error(tr("File format not recognized!"));
		}
	} else{
		emit error(tr("Invalid file path: ") + path);
	}
}

// if you pass cameraIndex=-1 (default), OpenCV will popup a simple
// camera selection widget
void ImageCapture::captureFromCamera( int cameraIndex ){
	this->stopCapture();
	this->_cvCap = cvCaptureFromCAM(cameraIndex);
	if( _cvCap ){
		this->_timer->setInterval(10);
		this->_timer->start();
	} else{
		emit error( tr("Invalid camera index: %1").arg(cameraIndex) );
	}
}

void ImageCapture::timerTick(){
        IplImage * image = cvQueryFrame(this->_cvCap);
//        Mat mat_img = imread( "/home/mike/Development/lenna.jpg", 1 );
       // IplImage * image = mat_img;
        if( image ){
		if( _processImage ){
			// simple edge detection algorithm
			const CvSize s = cvSize(image->width,image->height);
			IplImage * gray = cvCreateImage(s, image->depth, 1);	//!< grayscale image, one channel (this is what cvCanny expects)
			IplImage * edge = cvCreateImage(s, image->depth, 1);	//!< edge data buffer, one channel
			IplImage * final = cvCreateImage(s, image->depth, 3);	//!< final image with rendered edges
			cvZero(final);											//!< make sure final image is filled with black color
			cvCvtColor(image, gray, CV_BGR2GRAY);					//!< converting original BGR image to grayscale
			cvCanny(gray, edge, _threshold, 3*_threshold);			//!< detect edges on grayscale image and store them in edge buffer
			cvCopy(image, final, edge);								//!< copy image using edges as mask (if(mask[i]) final[i]=image[i])
			emit imageCaptured( convert(final) );
			cvReleaseImage(&gray);
			cvReleaseImage(&edge);
			cvReleaseImage(&final);
		} else{
			emit imageCaptured( convert(image) );
		}
	}
}

void ImageCapture::enableProcessing(bool on){
	_processImage = on;
}

void ImageCapture::setEdgeDetectorThreshold(int value){
	_threshold = value;
}

void ImageCapture::flipImage(bool on){
	_flipImage = on;
}

//
// here we will convert OpenCV image to QImage, copying the pixels row-by-row
// OpenCV stores the pixel data in row-major order, in BGR format
// QImage expects RGB, so we need to swap pixel valules
//
// for the sake of simplicity, only BGR24 images are handled (3 channels, 8-bit each)
//
// easier way is to pass image->imageData buffer directly to QImage constructor
// and then use QImage::rgbSwapped(), but its good to know how to manipulate pixel
// values directly
//
QImage ImageCapture::convert( IplImage * image ){
	QImage result;
	if( image ){
		uchar * buffer = NULL;
		QImage::Format format = QImage::Format_Invalid;
		if( IPL_DEPTH_8U == image->depth and 3 == image->nChannels ){
			// prepare image data buffer, we need enough space for WxH pixels, each pixel have 3 'uchar' values (r,g,b)
			buffer = (uchar *) malloc(image->width * image->height * 3 * sizeof(uchar));
			format = QImage::Format_RGB888;
			if( !buffer ){
				emit error(tr("Insufficient memory for image buffer!"));
				return result;
			}
			// copy image data row by row
			// image data layout is like this (w=image width, b(i) - blue component of i-th pixel in row, g - green, r - red):
			// row 0: [ b(0), g(0), r(0), b(1), g(1), r(1), ..., b(w-1), g(w-1), r(w-1) ]
			// row 1: [ b(w), g(w), r(w), b(w+1), g(w+1), r(w+2), ... ]
			// row 2: [ b(2*w), ... ]
			// then each row is placed one after another (h=image height):
			// image->imageData = [ row0, row1, row2, ..., row(h-1) ]
			for( int y = 0; y < image->height; ++y ){
				// each row pixels data starts at row*width position (if flipped, we count rows from h-1 to 0)
				const int srcRow = _flipImage ?  y*image->width : (image->height-y-1)*image->width;
				const int dstRow = y*image->width;
				for( int x = 0; x < image->width; ++x ){
					// multiply by 3, because each pixel occupies 3 entries in array
					const int srcPixel = (x+srcRow)*3;
					const int dstPixel = (x+dstRow)*3;
					// copy pixel value, swap r<->b channels
					buffer[ dstPixel + 0 ] = image->imageData[ srcPixel + 2 ];	// red
					buffer[ dstPixel + 1 ] = image->imageData[ srcPixel + 1 ];	// green
					buffer[ dstPixel + 2 ] = image->imageData[ srcPixel + 0 ];	// blue
				}
			}
		}else{
			emit error( tr("Format not supported: depth=%1, channels=%2").arg(image->depth).arg(image->nChannels));
			return result;
		}
		if( buffer ){
			// when creating QImage from buffer data, we need to make sure that
			// buffer remains valid until image is deleted, so we make explicit copy
			// and free the buffer immediately
			QImage * tmp = new QImage(buffer, image->width, image->height, format);
			result = tmp->copy();
			delete tmp;  free(buffer);
		}
	} else{
		emit error(tr("Image pointer is NULL"));
	}
	return result;
}
