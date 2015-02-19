#ifndef ALGORITHMROI_H
#define ALGORITHMROI_H


#include "common/common.h"
#include "DataHandling/flgpshandler.h"

class AlgorithmROI
{
public:
    // Standard constructor
    AlgorithmROI();

    // Method for applying roi detection on pic regarding gps data
    void applyAlgorithm(FlGPSHandler *a_flGPSHandler1, FlGPSHandler *a_flGPSHandler2, int a_indexFrame);

    // Return if a ROI has been found
    bool isROIComputed();

    // Return pointer to ROI
    cv::RotatedRect* getROI();

private:
    // ROI parameters
    bool mROIComputed;
    cv::RotatedRect mROI;

    // Algorithm attributes & methods
    cv::Mat C;
    cv::Mat SP;
    cv::Mat SN;
    cv::Mat ORN;
    float R;
    cv::Mat OIN;
    cv::Mat XIN;
    double DistToCurve(cv::Mat M, cv::Mat OIN, cv::Mat XIN);

};

#endif // ALGORITHMROI_H
