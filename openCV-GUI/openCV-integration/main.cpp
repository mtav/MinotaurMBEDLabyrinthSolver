#include <QtCore/QCoreApplication>

#include <cv.h>
#include <highgui.h>
#include <QVector>
#include <QDebug>

using namespace cv;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QVector<QVector<QString> > vectorOfVectorsOfStrings;

    for(int i = 0; i < 3; i++) //of course you might not want to init the vectors in a loop - this is just an example
    {
    QVector<QString> foo; //create a QVector of QStrings
    foo.push_back("fooo");
    foo.push_back("booo");
    vectorOfVectorsOfStrings.push_back(foo); //add the created vector as a line in your 2D vector
    }

    for(int i = 0; i < vectorOfVectorsOfStrings.size(); i++)
    {
    for(int j = 0; j < vectorOfVectorsOfStrings[i].size(); j++)
    {
    //do stuff with the QString vectorOfVectorsOfStrings[i][j]
    }
    }

    qDebug()<<vectorOfVectorsOfStrings;
    qDebug()<<"kOkok";

    QVector<QVector<int> > vectorOfVectorsOfInt;

    for(int i = 0; i < 3; i++) //of course you might not want to init the vectors in a loop - this is just an example
    {
    QVector<int> foo; //create a QVector of QStrings
    foo.push_back(2);
    foo.push_back(3);
    vectorOfVectorsOfInt.push_back(foo); //add the created vector as a line in your 2D vector
    }

    for(int i = 0; i < vectorOfVectorsOfInt.size(); i++)
    {
    for(int j = 0; j < vectorOfVectorsOfInt[i].size(); j++)
    {
    //do stuff with the QString vectorOfVectorsOfInt[i][j]
    }
    }

    qDebug()<<vectorOfVectorsOfInt;
    qDebug()<<"kOkok";

    Mat image;
    image = imread( argv[1], 1 );

    if( argc != 2 || !image.data )
      {
        printf( "No image data \n" );
        return -1;
      }

    namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
    imshow( "Display Image", image );
//
//    waitKey(0);

    return a.exec();
}
