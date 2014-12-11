#include "common/common.h"

void common::witchType(cv::Mat mat)
{
    int imgTypeInt = mat.type();
    std::string imgTypeString;
    switch (imgTypeInt)
        {
            case 0:
                imgTypeString = "8U";
                break;
            case 1:
                imgTypeString = "8S";
                break;
            case 2:
                imgTypeString = "16U";
                break;
            case 3:
                imgTypeString = "16S";
                break;
            case 4:
                imgTypeString = "32S";
                break;
            case 5:
                imgTypeString = "32F";
                break;
            case 6:
                imgTypeString = "64F";
                break;
            default:
                break;
        }
    DEBUG_MSG("Type: " << imgTypeString);
}

void common::showHist(cv::Mat hist, char* name)
{
    cv::Mat canvas = cv::Mat::ones(125, 256, CV_8UC3);

    float hmax = -1;
    for (int j = 0; j < 256-1; j++)
        hmax = hist.at<float>(j) > hmax ? hist.at<float>(j) : hmax;

    for (int j = 0, rows = canvas.rows; j < 256-1; j++)
    {
        cv::line(
                    canvas,
                    cv::Point(j, rows),
                    cv::Point(j, rows - (hist.at<float>(j) * rows/hmax)),
                    cv::Scalar(200,200,200),
                    1, 8, 0
                    );
    }

    imshow(name, canvas);
}

void common::addNoise(cv::Mat *a_mat,double a_amp){
     srand (time(NULL));
    for(int i = 0 ; i < a_mat->rows; i++){
            for (int j = 0; j < a_mat->cols; ++j){
                 a_mat->at<double>(i,j) += (((double)(rand() % 100)) / 100.0)*a_amp;
            }
        }
}
