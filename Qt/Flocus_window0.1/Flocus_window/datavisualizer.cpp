#include "datavisualizer.h"

using namespace cv;

DataVisualizer::DataVisualizer(QWidget *parent)
    : QWidget(parent)
    , width(640)
    , height(480)
{
    this->setMinimumSize(width,height);
    img = imread("/home/kiki/Images/Wallpaper/1338361597013.jpg", CV_LOAD_IMAGE_COLOR);
    imgQt = new QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);

    labelImage = new QLabel(this);
    labelImage->setPixmap(QPixmap::fromImage(*imgQt));
    labelImage->setAlignment(Qt::AlignCenter);
}

void DataVisualizer::updateImage(){
//    imgQt= new QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    labelImage->setPixmap(QPixmap::fromImage(*imgQt));
}
