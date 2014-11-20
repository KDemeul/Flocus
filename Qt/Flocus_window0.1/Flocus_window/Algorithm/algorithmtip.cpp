#include "algorithmtip.h"

// ------------ Constructor ----------------
AlgorithmTip::AlgorithmTip() :
    mTipComputed(false),
    mPercentTh(10.0),
    mPicNbPoint(0)
{

}

// ------------ applyAlgorithm ----------------
void AlgorithmTip::applyAlgorithm(cv::Mat *a_pic, cv::Rect *a_ROI, cv::Mat *a_Hj)
{
    DEBUG_MSG("Applying tip algo");
    if(a_ROI->width <= 0 || a_ROI->height <= 0)
        return;

    // Set image input
    cv::Mat img_grey(a_pic->size(), CV_8UC1);
    if(a_pic->type()!=CV_8UC1)
    {
        cv::cvtColor(*a_pic, img_grey, CV_RGB2GRAY);
        a_pic = &img_grey;
    }
    mPic = *a_pic;

    // Crop image
    cv::Mat picCropped = mPic(*a_ROI);
    picCropped.copyTo(mPicResized);

    // Normalize image
    cv::normalize(mPicResized, mPicResized, 0, 255, cv::NORM_MINMAX, CV_8UC1);

    // Count nb point in image
    mPicNbPoint = mPicResized.rows * mPicResized.cols;

    // Threshold pic
    convertPicToBoolMap();

    // Find the blob in the picture
    findBlobs();

    // Find the blobs intersecting the curve depicted by Hj

    // Find the biggest
    // Return the intersection of the biggest blob with the axis
}

// ------------ findBlobls ----------------
void AlgorithmTip::findBlobs()
{
    mBlobs.clear();

    cv::Mat label_image;
    mPicBinary.convertTo(label_image, CV_32SC1);

    int label_count = 2; // starts at 2 because 0,1 are used already

    for(int y=0; y < label_image.rows; y++) {
        int *row = (int*)label_image.ptr(y);
        for(int x=0; x < label_image.cols; x++) {
            if(row[x] != 255) {
                continue;
            }

            cv::Rect rect;
            cv::floodFill(label_image, cv::Point(x,y), label_count, &rect, 0, 0, 4);

            std::vector <cv::Point> blob;

            for(int i=rect.y; i < (rect.y+rect.height); i++) {
                int *row2 = (int*)label_image.ptr(i);
                for(int j=rect.x; j < (rect.x+rect.width); j++) {
                    if(row2[j] != label_count) {
                        continue;
                    }

                    blob.push_back(cv::Point(j,i));
                }
            }

            mBlobs.push_back(blob);

            label_count++;
        }
    }
}

// ------------ convertPicToBoolMap ----------------
void AlgorithmTip::convertPicToBoolMap()
{
    /* ------
     * Establish histogram
     * ------
     */

    int bins = 256;        // number of bins
    cv::Mat hist = cv::Mat::zeros(1,bins,CV_32SC1);          // array for storing the histogram

    for (int i = 0; i < mPicResized.rows; i++)
    {
        for (int j = 0; j < mPicResized.cols; j++)
        {
            uchar val = mPicResized.at<uchar>(i,j);
            hist.at<int>(val) += 1;
        }
    }

    /* ------
     * Count brightest and threshold
     * ------
     */

    int countBrightest = 0;
    int index = bins-1;
    while(countBrightest < mPercentTh*mPicNbPoint/ 100)
    {
        countBrightest += hist.at<int>(index);
        index--;
    }
    uchar threshValue = (uchar)index;

    cv::threshold(mPicResized, mPicBinary, threshValue, 255, cv::THRESH_BINARY);
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
