ABOUT
				
This program is a simple demonstration showing how to use OpenCV in Qt gui application.

 For more detailed info, check :
  - Qt documentation: http://doc.qt.nokia.com/latest/
  - OpenCV reference manual: http://opencv.willowgarage.com/documentation/index.html
 
 For comments or suggestions :
  - mail me: sebthestampede@gmail.com
  - send me a PM at Qt Centre forum (http://www.qtcentre.org/forum/ , user name: stampede)
 
 You can do whatever you want with this code.
 

 
INSTALLING
 
In order to compile, use qmake & make. You can pass cv=2 option to qmake (type: qmake cv=2), if you want to use OpenCV v2.2
You may want to edit OpenCV paths in .pro file to match your environment.

WARNING: You need to install OpenCV and Qt separately !



KNOWN ISSUES

Windows 7:
 - I've noticed that cvCaptureFromCAM(-1) can return NULL without opening camera selection widget.
	This seems random, and I don't know the solution.


