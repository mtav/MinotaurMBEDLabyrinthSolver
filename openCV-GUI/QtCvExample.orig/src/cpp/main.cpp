/*!
 * This program is a simple demonstration showing how to use OpenCV in Qt gui application.
 *
 * For more detailed info, check :
 *  - Qt documentation: http://doc.qt.nokia.com/latest/
 *  - OpenCV reference manual: http://opencv.willowgarage.com/documentation/index.html
 * 
 * For comments or suggestions :
 *  - mail me: sebthestampede@gmail.com
 *  - send me a PM at Qt Centre forum (http://www.qtcentre.org/forum/ , user name: stampede)
 * 
 * You can do whatever you want with this code.
 * 
 */

#include <QApplication>
#include "MainWidget.h"

int main( int argc, char ** argv ){
	QApplication a(argc,argv);
	MainWidget w;
	w.show();
	return a.exec();
}
