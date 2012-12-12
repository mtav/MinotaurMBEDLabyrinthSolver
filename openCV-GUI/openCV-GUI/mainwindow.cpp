#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>


#include <QVector>
#include <QGenericMatrix>
#include <QImage>
#include <cmath>
#include <QString>

#include "ImageCapture.h"


#include <iostream>
using namespace std;

void getImageSize(Mat& I, int& nRows, int&nCols)
{
   nRows = I.rows;
   nCols = I.cols;
}

void printImageInfo(Mat& I)
{
   int channels = I.channels();
   int nRows = I.rows;
   int nCols = I.cols * channels;

   cout << "channels: " << channels << endl;
   cout << "nRows: " << nRows << endl;
   cout << "nCols: " << nCols << endl;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->buttonLoadFile, SIGNAL(clicked()), this, SLOT(loadFile()));

    connect(ui->checkBoxEdgeDetect, SIGNAL(toggled(bool)), this, SLOT(enableEdgeDetector(bool)));

    this->_capture = new ImageCapture(this);
    connect(ui->sliderThreshold, SIGNAL(valueChanged(int)), this->_capture, SLOT(setEdgeDetectorThreshold(int)));
//    connect(ui->sliderThreshold, SIGNAL(valueChanged(int)), this->_capture, SLOT(update()));
    connect(ui->checkBoxFlip, SIGNAL(toggled(bool)), _capture, SLOT(flipImage(bool)));

    connect(_capture, SIGNAL(imageCaptured(const QImage&)), this, SLOT(displayImage(const QImage&)));

    ui->sliderThreshold->setEnabled( ui->checkBoxEdgeDetect->isChecked() );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_send_data_clicked()
{
//    qDebug()<<ui->lineEdit_serial_port->text();
//    qDebug().nospace()<<ui->plainTextEdit_data_to_send->toPlainText();
//    qDebug()<<"Hello world!";
    cout<<ui->plainTextEdit_data_to_send->toPlainText().toAscii().data()<<endl;
}

void calcMedianHSV(Mat& M_RGB)
{

  Mat I;
  cvtColor(M_RGB,I,CV_RGB2HSV);
  //cout<<"I = "<<M_RGB<<endl;

  int R=0;
  int G=0;
  int V=0;

    // accept only char type matrices
    CV_Assert(I.depth() != sizeof(uchar));

    const int channels = I.channels();
    switch(channels)
    {
    case 1:
        {
            for( int i = 0; i < I.rows; ++i)
                for( int j = 0; j < I.cols; ++j )
                    cout<<int(I.at<uchar>(i,j))<<endl;
            break;
        }
    case 3:
        {
         Mat_<Vec3b> _I = I;

         for( int i = 0; i < I.rows; ++i)
         {
            for( int j = 0; j < I.cols; ++j )
            {
              R+=int(_I(i,j)[0]);
              G+=int(_I(i,j)[1]);
              V+=int(_I(i,j)[2]);

            }
       }
         R = R/(float)(I.rows*I.cols);
         G = G/(float)(I.rows*I.cols);
         V = V/(float)(I.rows*I.cols);
          R=255;
          G=0;
          V=255;
         for( int i = 0; i < I.rows; ++i)
         {
            for( int j = 0; j < I.cols; ++j )
            {
              _I(i,j)[0]=R;
              _I(i,j)[1]=G;
              _I(i,j)[2]=V;
            }
                        //cout << endl;
         }
         I = _I;
         break;
        }
    }
      cvtColor(I,M_RGB,CV_HSV2RGB);

}

void calcMedianRGB(Mat& I)
{

//  Mat I;
//  cvtColor(M_RGB,I,CV_RGB2HSV);
  //cout<<"I = "<<M_RGB<<endl;

  int R=0;
  int G=0;
  int B=0;

    // accept only char type matrices
    CV_Assert(I.depth() != sizeof(uchar));

    const int channels = I.channels();
    switch(channels)
    {
    case 1:
        {
            for( int i = 0; i < I.rows; ++i)
                for( int j = 0; j < I.cols; ++j )
                    cout<<int(I.at<uchar>(i,j))<<endl;
            break;
        }
    case 3:
        {
         Mat_<Vec3b> _I = I;

         for( int i = 0; i < I.rows; ++i)
         {
            for( int j = 0; j < I.cols; ++j )
            {
              R+=int(_I(i,j)[0]);
              G+=int(_I(i,j)[1]);
              B+=int(_I(i,j)[2]);

            }
       }
         R = R/(float)(I.rows*I.cols);
         G = G/(float)(I.rows*I.cols);
         B = B/(float)(I.rows*I.cols);
         for( int i = 0; i < I.rows; ++i)
         {
            for( int j = 0; j < I.cols; ++j )
            {
              _I(i,j)[0]=R;
              _I(i,j)[1]=G;
              _I(i,j)[2]=B;
            }
                        //cout << endl;
         }
         I = _I;
         break;
        }
    }
//      cvtColor(I,M_RGB,CV_HSV2RGB);

}

QVector<QVector<int> > MainWindow::ImageMatrixToMazeMatrix(Mat& I)
{
    Mat I_HSV;
    cvtColor(I,I_HSV,CV_RGB2HSV);
    cout<<"I_HSV = "<<I_HSV;


    qDebug()<<"ImageMatrixToMazeMatrix";
    int nRows,nCols;
    getImageSize(I, nRows, nCols);
    qDebug()<<"nRows="<<nRows;
    qDebug()<<"nCols="<<nCols;

    int nSlicesX = ui->spinBox_nCols->value();
    int nSlicesY = ui->spinBox_nRows->value();

    qDebug()<<"nSlicesX="<<nSlicesX;
    qDebug()<<"nSlicesY="<<nSlicesY;

    Mat reduced_image(nSlicesY, nSlicesX, CV_8UC3, Scalar(0,0,255));

    QVector<QVector<int> > vectorOfVectorsOfInt;
    vectorOfVectorsOfInt.resize(nSlicesY);

    for(int i = 0; i < nSlicesY; i++) //of course you might not want to init the vectors in a loop - this is just an example
    {
        QVector<int> foo; //create a QVector of QStrings
        foo.resize(nSlicesX);
        vectorOfVectorsOfInt[i]=foo; //add the created vector as a line in your 2D vector
    }

    // accept only char type matrices
    CV_Assert(I.depth() != sizeof(uchar));

    const int channels = I.channels();
    switch(channels)
    {
    case 1:
        {
            for( int i = 0; i < I.rows; ++i)
                for( int j = 0; j < I.cols; ++j )
                    cout<<int(I.at<uchar>(i,j))<<endl;
            break;
        }
    case 3:
        {
         Mat_<Vec3b> _I = I;
         Mat_<Vec3b> _reduced_image = reduced_image ;

         int rect_height = ceil(I.rows/(float)nSlicesY);
         int rect_width = ceil(I.cols/(float)nSlicesX);

         qDebug()<<"I.rows = "<<I.rows;
         qDebug()<<"I.cols = "<<I.cols;
         qDebug()<<"rect_height  = "<<rect_height ;
         qDebug()<<"rect_width  = "<<rect_width ;

         for( int sub_i = 0; sub_i < nSlicesY; ++sub_i)
         {
            for( int sub_j = 0; sub_j < nSlicesX; ++sub_j )
            {
                int i = sub_i*rect_height;
                int j = sub_j*rect_width;
//                qDebug()<<"sub_i,sub_j"<<sub_i<<","<<sub_j<<" -> "<<"i,j = "<<i<<","<<j;
                if(i<I.rows && j<I.cols) {
                    //         for( int i = 0; i < I.rows; ++i)
                    //         {
                    //            for( int j = 0; j < I.cols; ++j )
                    //            {
                    int loc_rect_height = rect_height;
                    int loc_rect_width = rect_width;
                    if (i+rect_height>=I.rows) loc_rect_height = I.rows-i-1;
                    if (j+rect_width>=I.cols) loc_rect_width = I.cols-j-1;

                                    if (i+loc_rect_height<I.rows && j+loc_rect_width<I.cols) {
                                        QString subsection_filename = QString("subsection-i_%1-j_%2.bmp").arg(i).arg(j);
                    //                    qDebug()<<"ImageMatrixToMazeMatrix: saving "<<subsection_filename;
                                        Mat subsection(_I, Rect(j, i, loc_rect_width, loc_rect_height) ); // using a rectangle
                                        imwrite( subsection_filename.toAscii().data(), subsection );

                                        //calcMedian(subsection);
                                        calcMedianRGB(subsection);
//                                        calcMedianHSV(subsection);
                                    }

//                                    Mat I_HSV;
//                                    cvtColor(subsection,I_HSV,CV_RGB2HSV);
//                                    calcMediumHSV_Value()

                                    int H = 255;
                                    int S = 255;
                                    int V = 255;

                                        _reduced_image(sub_i,sub_j)[0]=int(_I(i,j)[0]);
                                        _reduced_image(sub_i,sub_j)[1]=int(_I(i,j)[1]);
                                        _reduced_image(sub_i,sub_j)[2]=int(_I(i,j)[2]);

                                                       if (int(_I(i,j)[0])==255 && int(_I(i,j)[1])==255 && int(_I(i,j)[2])==255) {
                                                                    vectorOfVectorsOfInt[sub_i][sub_j]=0;
                                                               }
                                                       else if (int(_I(i,j)[0])==0 && int(_I(i,j)[1])==0 && int(_I(i,j)[2])==0) {
                                                                    vectorOfVectorsOfInt[sub_i][sub_j]=1;
                                                               }
                                                       else if (int(_I(i,j)[0])==0 && int(_I(i,j)[1])==0 && int(_I(i,j)[2])==255) {
                                                                    vectorOfVectorsOfInt[sub_i][sub_j]=2;
                                                               }
                                                       else if (int(_I(i,j)[0])==255 && int(_I(i,j)[1])==0 && int(_I(i,j)[2])==0) {
                                                                    vectorOfVectorsOfInt[sub_i][sub_j]=3;
                                                               }

                }
            }
         }
         I = _I;
         reduced_image = _reduced_image;
         break;
        }
    }
    QString reduced_filename = "/home/mike/reduced_image.bmp";
    imwrite(reduced_filename.toAscii().data(), reduced_image);

    QImage qimage(reduced_filename);
    displayImage_reduced(qimage);

    return(vectorOfVectorsOfInt);
}

//void printMaze( const char maze_in[][MAZE_WIDTH])
//{
//    for(int y = 0; y < MAZE_HEIGHT; y++){
//      for (int x = 0; x < MAZE_WIDTH; x++){
//        cout<<int(maze_in[y][x]);
//      }
//      cout << endl;
//    }
//}

void printMaze( char** maze_in, int width, int height)
{
    for(int y = 0; y < height; y++){
      for (int x = 0; x < width; x++){
        cout<<int(maze_in[y][x]);
      }
      cout << endl;
    }
}

void printMaze( QVector<QVector<int> > maze_in)
{
    for(int i = 0; i < maze_in.size(); i++)
    {
    for(int j = 0; j < maze_in[i].size(); j++)
    {
        cout<<int(maze_in[i][j]);

    }
    cout << endl;
    }

//    for(int y = 0; y < height; y++){
//      for (int x = 0; x < width; x++){
//        cout<<int(maze_in[y][x]);
//      }
//      cout << endl;
//    }
}

QString MazeToString( QVector<QVector<int> > maze_in, bool newlines)
{
    QString str;
   // str<<"hjhjh";
    for(int i = 0; i < maze_in.size(); i++)
    {
        for(int j = 0; j < maze_in[i].size(); j++)
        {
 //           str<<int(maze_in[i][j]);
//            str += QString(maze_in[i][j]);
            str += QString("%1").arg(maze_in[i][j]);
//            QString::number(iNumber);
        }
        if(newlines) str += "\n";
    }

    return(str);
}

QString MainWindow::getStringFromImage(char* filename, bool newlines)
{
    qDebug()<<"getStringFromImage";
  Mat image_in;
  image_in = imread( filename, 1 );
  QVector<QVector<int> > maze_matrix = ImageMatrixToMazeMatrix(image_in);
//  QGenericMatrix maze_matrix = ImageMatrixToMazeMatrix(image_in);
//    qDebug()<<maze_matrix;
//  printMaze(maze_matrix, width, height);
//      printMaze(maze_matrix);
  return(MazeToString(maze_matrix, newlines));
}

int MainWindow::printMazeFromImage(char* filename)
{
  Mat image_in;
  image_in = imread( filename, 1 );
  QVector<QVector<int> > maze_matrix = ImageMatrixToMazeMatrix(image_in);
//  QGenericMatrix maze_matrix = ImageMatrixToMazeMatrix(image_in);
//    qDebug()<<maze_matrix;
//  printMaze(maze_matrix, width, height);
      printMaze(maze_matrix);
  return(0);
}

void MainWindow::on_actionOpen_triggered()
{

    qDebug()<<"on_actionOpen_triggered...";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"~/Pictures",tr("Images (*.png *.xpm *.jpg *.bmp)"));

//     direct loading
    QImage qimage(fileName);
    displayImage(qimage);


    //QString      string = "this is string";
    QByteArray   bytes  = fileName.toAscii();
    //const char * ptr    = bytes.data();

    Mat mat_img;
    mat_img = imread( bytes.data(), 1 );

    if( !mat_img.data )
      {
        qDebug()<< "No image data";
        return;
      }

//    // indirect loading
//    QImage qimage;
//    IplImage ipl_img = mat_img;
//    qimage = this->_capture->convert(&ipl_img);
//    displayImage(qimage);

//    namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
//    imshow( "Display Image", mat_img );

//    printMazeFromImage(bytes.data());
    ui->plainTextEdit_data_to_send->setPlainText(getStringFromImage(bytes.data(),ui->checkBox_LF->isChecked()));
    //waitKey(0);

//    this->_capture->update();

    return;
}

void MainWindow::displayImage( const QImage& image ){
        const QPixmap pix = QPixmap::fromImage(image);
        const QSize size = ui->displayLabel->size();
        this->ui->displayLabel->setPixmap(pix.scaled(size,Qt::KeepAspectRatio));
}

void MainWindow::displayImage_reduced( const QImage& image ){
        const QPixmap pix = QPixmap::fromImage(image);
        const QSize size = ui->displayLabel_reduced->size();
        this->ui->displayLabel_reduced->setPixmap(pix.scaled(size,Qt::KeepAspectRatio));
}

void MainWindow::enableEdgeDetector(bool on){
        this->ui->sliderThreshold->setEnabled(on);
        this->_capture->setEdgeDetectorThreshold(ui->sliderThreshold->value());
        this->_capture->enableProcessing(on);
}

void MainWindow::loadFile(){
        const QString path = QFileDialog::getOpenFileName(this,tr("Select file"),"",tr("Images (*.png *.xpm *.jpg *.bmp)"));
        if( !path.isEmpty() ){
                this->_capture->captureFromFile(path);
        }
}
