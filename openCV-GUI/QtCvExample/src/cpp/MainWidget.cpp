#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "ImageCapture.h"
#include <QFileDialog>
#include <QMessageBox>

MainWidget::MainWidget( QWidget * parent )
	:QWidget(parent), _ui(new Ui::MainWidget)
{
	this->_ui->setupUi(this);
	connect(_ui->buttonCamera, SIGNAL(clicked()), this, SLOT(startCamera()));
	connect(_ui->buttonLoadFile, SIGNAL(clicked()), this, SLOT(loadFile()));
	connect(_ui->checkBoxEdgeDetect, SIGNAL(toggled(bool)), this, SLOT(enableEdgeDetector(bool)));
	this->_capture = new ImageCapture(this);
	connect(_capture, SIGNAL(error(const QString&)), this, SLOT(captureError(const QString&)));
	connect(_capture, SIGNAL(imageCaptured(const QImage&)), this, SLOT(displayImage(const QImage&)));
	connect(_ui->sliderThreshold, SIGNAL(valueChanged(int)), _capture, SLOT(setEdgeDetectorThreshold(int)));
	connect(_ui->checkBoxFlip, SIGNAL(toggled(bool)), _capture, SLOT(flipImage(bool)));
	_ui->sliderThreshold->setEnabled( _ui->checkBoxEdgeDetect->isChecked() );
	_capture->flipImage( _ui->checkBoxFlip->isChecked() );
}

MainWidget::~MainWidget(){
	delete _capture;
	delete _ui;
}

void MainWidget::startCamera(){
	this->_capture->captureFromCamera();
}

void MainWidget::loadFile(){
        const QString path = QFileDialog::getOpenFileName(this,tr("Select file"),"",tr("Images (*.png *.xpm *.jpg *.bmp)"));
	if( !path.isEmpty() ){
		this->_capture->captureFromFile(path);
	}
}

void MainWidget::enableEdgeDetector(bool on){
	this->_ui->sliderThreshold->setEnabled(on);
	this->_capture->setEdgeDetectorThreshold(_ui->sliderThreshold->value());
	this->_capture->enableProcessing(on);
}

void MainWidget::displayImage( const QImage& image ){
	const QPixmap pix = QPixmap::fromImage(image);
	const QSize size = _ui->displayLabel->size();
	this->_ui->displayLabel->setPixmap(pix.scaled(size,Qt::KeepAspectRatio));
} 

void MainWidget::captureError( const QString& text ){
	QMessageBox::critical(this,windowTitle(), text);
}
