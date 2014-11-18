#include "algorithmtip.h"

// ------------ Constructor ----------------
AlgorithmTip::AlgorithmTip() :
    mTipComputed(false)
{

}

// ------------ applyAlgorithm ----------------
void AlgorithmTip::applyAlgorithm(cv::Mat *a_pic, cv::Mat *a_Hj)
{
    // Compute blob on the pic

    // creation
    cv::SimpleBlobDetector * blob_detector;
    blob_detector = new cv::SimpleBlobDetector();
    blob_detector->create("SimpleBlobDetector");

    // detect
    std::vector<cv::KeyPoint> keypoints;
    blob_detector->detect(*a_pic, keypoints);

    foreach(cv::KeyPoint keypoint, keypoints)
    {
        DEBUG_MSG("blob : " << keypoint.size << " " << keypoint.pt << " " << keypoint.angle);
    }

    // Find the blobs intersecting the curve depicted by Hj
    // Find the biggest
    // Return the intersection of the biggest blob with the axis
}

// ------------ isTipComputed ----------------
bool AlgorithmTip::isTipComputed()
{
    return mTipComputed;
}

// ------------ getTip ----------------
cv::Point* AlgorithmTip::getTip()
{
    return &mPointTip;
}
