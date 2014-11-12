#ifndef ALGORITHMRANSAC_H
#define ALGORITHMRANSAC_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv/cv.h"
#include "opencv2/imgproc/imgproc.hpp"

#include <set>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <limits>       // std::numeric_limits

#include "common.h"

class AlgorithmRansac
{
public:
    AlgorithmRansac(int a_ransacNbPoint);

    void applyAlgorithm(cv::Mat a_pic, cv::Rect a_regionOfInterest);

    void setAreaOfInterest(cv::Rect a_regionOfInterest);
    void setEta(double a_eta);
    void setRho(double a_rho);

private:
    //-----> Param

    // Global image
    cv::Mat mPic;

    // Image for work
    cv::Rect mRegionOfInterest;
    cv::Mat mPicResized;
    cv::Mat mPicBool;
    std::vector<cv::Point> mIndexThresh;

    // RANSAC parameters
    int mRansacNbPoint; // Nb of parameter for which curve as to be fitted
    int mPicNbPoint; // Nb of point in the ROI
    double mPercentTh; // fraction of pixel kept
    double mEta; // η is a control parameter that defines the probability that the best model parameters will not be found.
    double mRho; // rho is a control parameter that defines the minimum distance of a point to be an inlier
    double mJ; // current nb of iteration

    //-----> Result
    double** HRNS; // coefficient of the curve approximating the electrode axis
    double   CRNS; // cost of the model
    double** DRNS; // set of control points
    double** TRNS; // value of parameters t for control points
    std::vector<cv::Point> mInliers;

    //-----> Methods
    void convertPicToBoolMap();
    void createIndexThresh();
    SetPoint getRandPoints();
    bool isAPotentialCurve(SetPoint a_potentialSet);

};

#endif // ALGORITHMRANSAC_H
