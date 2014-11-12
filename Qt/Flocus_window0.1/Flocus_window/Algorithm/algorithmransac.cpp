#include "algorithmransac.h"

AlgorithmRansac::AlgorithmRansac()
    : mEta(0.001),
      mRho(1.0),
      mPercentTh(10.0)
{
}

void AlgorithmRansac::applyAlgorithm(cv::Mat a_pic, cv::Rect a_regionOfInterest, int a_ransacNbPoint)
{
    mPic = a_pic;
    mRansacNbPoint = a_ransacNbPoint;
    setAreaOfInterest(a_regionOfInterest);
    mPicNbPoint = mPicResized.rows * mPicResized.cols;
    convertPicToBoolMap();
}

void AlgorithmRansac::setAreaOfInterest(cv::Rect a_regionOfInterest)
{
    mRegionOfInterest = a_regionOfInterest;
    cv::Mat picCropped = mPic(mRegionOfInterest);
    picCropped.copyTo(mPicResized);
}

void AlgorithmRansac::setEta(double a_eta)
{
    mEta = a_eta;
}

void AlgorithmRansac::setRho(double a_rho)
{
    mRho = a_rho;
}


void AlgorithmRansac::convertPicToBoolMap()
{
    /* ------
     * Establish histogram
     * ------
     */

    // Keep a fraction of brightest pixels
    cv::Mat hist;

    // Establish the number of bins
    int histSize = 256;

    // Set the range
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    // Compute the histogram:
    calcHist( &mPicResized, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );

    /* ------
     * Count brightest and threshold
     * ------
     */

    int countBrightest = 0;
    int index = 255;
    while(countBrightest < mPercentTh*mPicNbPoint/ 100)
    {
        countBrightest += hist.at<int>(index);
    }

    double threshValue = index;

    cv::threshold(mPicResized, mPicBool, threshValue, 255, cv::THRESH_BINARY);

    cv::imshow("Pouet", mPicBool);
    cv::waitKey(0);
}
