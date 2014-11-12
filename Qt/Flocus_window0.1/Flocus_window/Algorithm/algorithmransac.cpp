#include "algorithmransac.h"

AlgorithmRansac::AlgorithmRansac(int a_ransacNbPoint)
    : mEta(0.001),
      mRho(1.0),
      mPercentTh(10.0),
      mRansacNbPoint(a_ransacNbPoint)
{
    // INITIALIZATION
    CRNS = std::numeric_limits<double>::max();

    // Compute the initial number of iterations J:
    double ksi = 0.5;
    mJ = log(mEta) / log(1 - pow(ksi,2));

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

    while(j < 1)
    {
    //  Select randomly a subset Sj ⊂ Xe, |Sj| = mRansacNbPoint
    SetPoint Sj = getRandPoints();

    bool acceptedPoint = isAPotentialCurve(Sj);

    if(!acceptedPoint)
        continue;

    // Find Dj by ordering points in Sj
       // TODO

    SetPoint Dj = Sj;

    // Compute coefficient matrix Hj from the set Dj
    cv::Mat Hj;
    cv::Mat Tj;
    fillMatricesHjTj(&Hj,&Tj,&Dj);

    // Check consistency of a randomly selected point x from Xe with the model c(t; Hj)
    // If x passes, then continue; otherwise increment j and loop

    int indexRand = rand() % mIndexThresh.size();
    cv::Point randPoint = mIndexThresh.at(indexRand);

    double d = DistToCurve(&Hj,&Dj,&Tj,&randPoint);

    if (d > mRho)
    {
        j = j + 1 ;
        continue;
    }

    j++;
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
    if(a_potentialSet.size() == mRansacNbPoint)
        return true;
    return true;
}

void AlgorithmRansac::fillMatricesHjTj(cv::Mat *Hj, cv::Mat *Tj, SetPoint *Dj)
{
    if(mRansacNbPoint == 2)
    {
        // Get point
        SetPoint::iterator it = Dj->begin();
        cv::Point D1 = *it;
        std::advance(it,1);
        cv::Point D2 = *it;
        cv::Mat DjMat = (cv::Mat_<double>(2,2) << D1.x, D2.x, D1.y, D2.y);


        // Set Tj
        double t1 = 0;
        double t2 = cv::norm(D1-D2);
        *Tj = (cv::Mat_<double>(2, 2) << 1,1,t1,t2);

        // Set Hj
        *Hj = DjMat * Tj->inv();
    }
    else
        DEBUG_MSG("WRONG PARAMETER IN RANSAC FITTING : mRansacNbPoint != 2 (mRansacNbPoint = " << mRansacNbPoint << ")");
}

double AlgorithmRansac::DistToCurve(cv::Mat *Hj,SetPoint *Dj,cv::Mat *Tj,cv::Point *randPoint)
{
    if(mRansacNbPoint == 2)
    {
        // Set points
        SetPoint::iterator it = Dj->begin();
        cv::Point D1 = *it;
        std::advance(it,1);
        cv::Point D2 = *it;

        cv::Point vL = D2 - D1;
        cv::Point w = *randPoint-D1;

        return abs(vL.x*w.y - vL.y * w.x) / cv::norm(vL);
    }
    else
        return std::numeric_limits<double>::max();
}
