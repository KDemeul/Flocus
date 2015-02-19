#ifndef ROIVISUALIZER_H
#define ROIVISUALIZER_H

#include <QWidget>

#include "common/common.h"
#include "DataHandling/flgpshandler.h"
#include "Algorithm/algorithmroi.h"

class ROIVisualizer : public QWidget
{
    Q_OBJECT
public:
    // stadard constructor
    ROIVisualizer(QWidget *parent = 0);

    // method for applying algorithm to the image imgCV regarding gps data
    void applyAndDraw(cv::Mat *a_imgToDrawOn,cv::Mat *a_imgToProcess, FlGPSHandler *a_flGPSHandler1, FlGPSHandler *a_flGPSHandler2, int a_indexFrame);

    // method for drawing the ROI on the imgToDrawOn
    void drawROI(cv::Mat *a_imgToDrawOn, cv::RotatedRect *a_ROI);
signals:

public slots:

private:
   // algorithm linked to the visualizer
   AlgorithmROI *mAlgorithmROI;

   // information about the algorithm
   bool mIsEnabled;
   int mIndexLastFrameProcessed;

};

#endif // ROIVISUALIZER_H
