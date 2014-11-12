#ifndef ALGORITHMRANSAC_H
#define ALGORITHMRANSAC_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv/cv.h"
#include "opencv2/imgproc/imgproc.hpp"

class AlgorithmRansac
{
public:
    AlgorithmRansac();

    void applyAlgorithm(cv::Mat a_pic, cv::Rect a_regionOfInterest, int a_ransacNbPoint);

    void setAreaOfInterest(cv::Rect a_regionOfInterest);
    void setEta(double a_eta);
    void setRho(double a_rho);

private:
    // Param
    cv::Mat mPic;

    cv::Rect mRegionOfInterest;
    cv::Mat mPicResized;
    cv::Mat mPicBool;

    int mRansacNbPoint; // Nb of parameter for which curve as to be fitted
    int mPicNbPoint; // Nb of point in the ROI
    double mPercentTh; // fraction of pixel kept
    double mEta; // η is a control parameter that defines the probability that the best model parameters will not be found.
    double mRho; // rho is a control parameter that defines the minimum distance of a point to be an inlier

    // Result
    double** HRNS; // coefficient of the curve approximating the electrode axis
    double   CRNS; // cost of the model
    double** DRNS; // set of control points
    double** TRNS; // value of parameters t for control points

    // Methods

    void convertPicToBoolMap();
};

#endif // ALGORITHMRANSAC_H
