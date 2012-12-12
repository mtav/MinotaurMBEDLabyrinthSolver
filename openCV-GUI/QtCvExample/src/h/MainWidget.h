#ifndef _MAIN_WIDGET_H_
#define _MAIN_WIDGET_H_

#include <QWidget>

class Ui_MainWidget;
class QImage;
class ImageCapture;

class MainWidget : public QWidget{
	Q_OBJECT
public:
	MainWidget( QWidget * parent = NULL );
	~MainWidget();
public slots:
	void startCamera();
	void loadFile();
	void enableEdgeDetector(bool on);
	void displayImage( const QImage& image );
protected slots:
	void captureError( const QString& text );
protected:
	Ui_MainWidget * _ui;
	ImageCapture * _capture;
};

#endif
