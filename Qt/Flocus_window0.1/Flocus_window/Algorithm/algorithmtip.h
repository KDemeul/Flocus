#ifndef ALGORITHMTIP_H
#define ALGORITHMTIP_H

#include "common/common.h"
#include "common/xmlhandler.h"

class AlgorithmTip
{
public:
    // Standard constructor
    AlgorithmTip();

    // Method for applying tip detection on pic regarding curve
    // with parameter &Hj
   void applyAlgorithm(cv::Mat *a_pic, cv::Rect *a_ROI, cv::Mat *a_Hj, int a_indexFrame);

    // Return if a tip has been found
    bool isTipComputed();

    // Return pointer to tip point
    cv::Point* getTip();

    // Set needle insertion direction
    void setDirection(ORIENTATION_NEEDLE a_ori);

private:
    // tip parameters
    bool mTipComputed;
    cv::Point mPointTip;
    ORIENTATION_NEEDLE mOrientationNeedle;
    bool mHjChanged;

    // Algorithm attributes & methods
    float mPercentTh;
    int mPicNbPoint;
    cv::Mat mPic;
    cv::Mat mPicResized;
    cv::Mat mPicBinary;
    cv::Mat mHj;
    std::vector<Blob> mBlobs;
    std::vector<Blob> mBlobsOnLine;
    Blob mBlobWithTip;

    void findBlobs();
    void convertPicToBoolMap();
    bool isPointOnCurve(cv::Point P);

};

#endif // ALGORITHMTIP_H
