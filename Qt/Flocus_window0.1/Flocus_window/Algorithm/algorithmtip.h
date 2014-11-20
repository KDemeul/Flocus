#ifndef ALGORITHMTIP_H
#define ALGORITHMTIP_H

#include "common/common.h"

class AlgorithmTip
{
public:
    // Standard constructor
    AlgorithmTip();

    // Method for applying tip detection on pic regarding curve
    // with parameter &Hj
   void applyAlgorithm(cv::Mat *a_pic, cv::Rect *a_ROI, cv::Mat *a_Hj);

    // Return if a tip has been found
    bool isTipComputed();

    // Return pointer to tip point
    cv::Point* getTip();

private:
    // tip parameters
    bool mTipComputed;
    cv::Point mPointTip;

    // Algorithm attributes & methods
    float mPercentTh;
    int mPicNbPoint;
    cv::Mat mPic;
    cv::Mat mPicResized;
    cv::Mat mPicBinary;
    std::vector<Blob> mBlobs;
    void findBlobs();
    void convertPicToBoolMap();

};

#endif // ALGORITHMTIP_H
