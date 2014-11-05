#include "datavisualizer.h"

DataVisualizer::DataVisualizer(QWidget *parent)
    : QWidget(parent)
    , width(640)
    , height(480)
    , indexCurrentFrame(0)
{
    flDataHandler = new FlDataHandler();

    this->setMinimumSize(width,height);

    labelImage = new QLabel(this);
    labelImage->setAlignment(Qt::AlignCenter);
}

void DataVisualizer::updateImage(){
    width = flDataHandler->w;
    height = flDataHandler->h;

    img = cv::Mat::zeros(height,width, CV_32FC1);

    for (int x = 0; x < height; ++x) {
        for (int y = 0; y < width; ++y) {
            img.at<float>(x,y) = flDataHandler->allPictures[indexCurrentFrame][x][width-y];
        }
    }

    cv::Mat matScaled;
    img.convertTo(matScaled, CV_8UC1, 255, 0);

    imgQt = QImage((uchar*) matScaled.data, matScaled.cols, matScaled.rows, matScaled.step, QImage::Format_Indexed8);

    labelImage->setPixmap(QPixmap::fromImage(imgQt));
    labelImage->setAlignment(Qt::AlignCenter);
    labelImage->setMinimumSize(width,height);
}

void DataVisualizer::setDataHandler(FlDataHandler* a_flDataHandler)
{
    flDataHandler = a_flDataHandler;
}

void DataVisualizer::setFrame(int a_indexFrame)
{
    if(flDataHandler->fileLoaded)
    {
        if(a_indexFrame < flDataHandler->nframes && a_indexFrame > -1)
            indexCurrentFrame = a_indexFrame;
        updateImage();
    }
}

void DataVisualizer::nextFrame()
{
    int desiredIndex = indexCurrentFrame + 1;
    setFrame(desiredIndex);
}

void DataVisualizer::previousFrame()
{
    int desiredIndex = indexCurrentFrame - 1;
    setFrame(desiredIndex);
}


void DataVisualizer::firstFrame()
{
    setFrame(0);
}

void DataVisualizer::lastFrame()
{
    setFrame(flDataHandler->nframes-1);
}

void DataVisualizer::play()
{
    if(flDataHandler->fileLoaded)
    {
        isPlaying = !isPlaying;

        cv::namedWindow("pouet");
        cv::resizeWindow("pouet",0,0);
        while(isPlaying && indexCurrentFrame < flDataHandler->nframes-1)
        {
            nextFrame();
            cv::waitKey(1000/flDataHandler->ss);
        }
        cv::destroyAllWindows();
    }
}

void DataVisualizer::pause()
{
    isPlaying = false;
}


void DataVisualizer::sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

