#ifndef TIPVISUALIZER_H
#define TIPVISUALIZER_H

#include <QWidget>
#include "common/common.h"
#include "Algorithm/algorithmtip.h"

class TipVisualizer : public QWidget
{
public:
    // standard Constructor
    TipVisualizer(QWidget *parent = 0);

    // method for applying algorithm to the image imgCv regarding curve parameter Hj
    void applyAndDraw(cv::Mat *a_imgCV, cv::Mat *a_Hj);

signals:

public slots:
    // slot for toggeling the algorithm status
    void enable(bool a_isEnable);

private:
    // algorithm linked to the visualizer
    AlgorithmTip *mAlgorithmTip;

    // information about the algorithm
    bool mIsEnabled;
    int mIndexLastFrameProcessed;

    //  this draw the tip on imgCV at ptTip
    void drawTip(cv::Mat* a_imgCV, cv::Point *a_ptTip);
};

#endif // TIPVISUALIZER_H
