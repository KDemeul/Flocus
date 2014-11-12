#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv/cv.h"

class Algorithm
{
public:
    Algorithm();

    virtual void applyAlgorithm(cv::Mat pic) = 0;
};

#endif // ALGORITHM_H
