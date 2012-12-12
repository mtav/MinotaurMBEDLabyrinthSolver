#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <cv.h>
#include <highgui.h>

using namespace cv;

namespace Ui {
    class MainWindow;
}

class ImageCapture;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QVector<QVector<int> > ImageMatrixToMazeMatrix(Mat& I);
    QString getStringFromImage(char* filename, bool newlines);
    int printMazeFromImage(char* filename);

protected:
    void changeEvent(QEvent *e);
    ImageCapture * _capture;

private:
    Ui::MainWindow *ui;

private slots:
    void on_actionOpen_triggered();
    void on_pushButton_send_data_clicked();

public slots:
        void displayImage( const QImage& image );
        void displayImage_reduced( const QImage& image );
        void enableEdgeDetector(bool on);
        void loadFile();

};

#endif // MAINWINDOW_H
