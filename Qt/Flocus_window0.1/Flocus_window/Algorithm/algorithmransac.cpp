#include "algorithmransac.h"

AlgorithmRansac::AlgorithmRansac(int a_ransacNbPoint)
    : mEta(0.001),
      mRho(1.0),
      mPercentTh(10.0),
      mRansacNbPoint(a_ransacNbPoint)
{
    // INITIALIZATION
    CRNS = std::numeric_limits<double>::max();
    HRNS = (double**) malloc(sizeof(double*) * mRansacNbPoint);
    DRNS = (double**) malloc(sizeof(double*) * mRansacNbPoint);
    TRNS = (double**) malloc(sizeof(double*) * mRansacNbPoint);
    for(int i = 0; i < mRansacNbPoint ; i++)
    {
        HRNS[i] = (double*) malloc(sizeof(double) * mRansacNbPoint);
        DRNS[i] = (double*) malloc(sizeof(double) * mRansacNbPoint);
        TRNS[i] = (double*) malloc(sizeof(double) * mRansacNbPoint);
        for(int j=0 ; j < mRansacNbPoint ; j++)
        {
            HRNS[i][j] = std::numeric_limits<double>::max();
            DRNS[i][j] = std::numeric_limits<double>::max();
            TRNS[i][j] = std::numeric_limits<double>::max();
        }
    }

    // Compute the initial number of iterations J:
    double ksi = 0.5;
    mJ = log(mEta) / log(1 - ksi^2);
}

void AlgorithmRansac::applyAlgorithm(cv::Mat a_pic, cv::Rect a_regionOfInterest)
{
    // Set image input
    mPic = a_pic;

    // Crop image
    setAreaOfInterest(a_regionOfInterest);
    DEBUG_MSG("IMAGE CROPPED");

    // Normalize image
    cv::normalize(mPicResized,mPicResized,0,255,cv::NORM_MINMAX);

    // Count nb point in image
    mPicNbPoint = mPicResized.rows * mPicResized.cols;

    // Threshold pic
    convertPicToBoolMap();
    DEBUG_MSG("IMAGE CONVERTED TO BOOLMAP");

    // Establish index thresh
    createIndexThresh();
    DEBUG_MSG("INDEX MAP THRESH CREATED (" << (mIndexThresh.size() * 100) / mPicNbPoint << "%)");

    int j=0; // nb iter inside loop

    while(j < mJ)
    {
    //  Select randomly a subset Sj ⊂ Xe, |Sj| = mRansacNbPoint
    SetPoint Sj = getRandPoints();

    bool acceptedPoint = isAPotentialCurve(Sj);


    }




    // DEBUG DISPLAY
//    cv::imshow("Pic Extracted", mPicResized);
//    cv::imshow("Pic thresholded", mPicBool);
//    cv::waitKey(0);
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
    float range[] = { 0, histSize } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    // Compute the histogram:
    calcHist( &mPicResized, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );

    /* ------
     * Count brightest and threshold
     * ------
     */

    int countBrightest = 0;
    int index = histSize-1;
    while(countBrightest < mPercentTh*mPicNbPoint/ 100)
    {
        countBrightest += hist.at<float>(index);
        index--;
    }

    double threshValue = index;

    cv::threshold(mPicResized, mPicBool, threshValue, 255, cv::THRESH_BINARY);
}

void AlgorithmRansac::createIndexThresh()
{
    for(int i=0; i < mPicBool.rows; i++)
    {
        for(int j=0; j < mPicBool.cols; j++)
        {
            if(mPicBool.at<float>(i,j))
                mIndexThresh.push_back(cv::Point(i,j));
        }
    }
}

SetPoint AlgorithmRansac::getRandPoints()
{
    std::set<cv::Point,comparePoints> results;
    while(results.size() != mRansacNbPoint)
    {
        int index = rand() % mIndexThresh.size();
        results.insert(mIndexThresh.at(index));
    }
    return results;
}

bool AlgorithmRansac::isAPotentialCurve(SetPoint a_potentialSet) // STUPID FUNCTION
{
    if(a_potentialSet == mRansacNbPoint)
        return true;
    return true;
}
