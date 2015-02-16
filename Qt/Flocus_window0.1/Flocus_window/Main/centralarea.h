#ifndef CENTRALAREA_H
#define CENTRALAREA_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSlider>
#include <QProgressDialog>

#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv/cv.h"

#include "Main/algorithmarea.h"
#include "Visualizers/datavisualizer.h"
#include "DataHandling/fldatahandler.h"
#include "DataHandling/flgpshandler.h"


class CentralArea : public QWidget
{
    Q_OBJECT

public:
    CentralArea(QWidget *parent = 0);
    AlgorithmArea* getAlgorithmArea();
    DataVisualizer* getDataVisualizer();
    void setRansacParameters(int a_posX, int a_poxY, int a_width, int a_height, bool a_isEnable, int a_ransacRate);
    void setTipParameters(ORIENTATION_NEEDLE a_dir, bool a_isEnable);
    void setKalmanParameters(bool a_isEnable);
    std::vector<cv::Mat> getKalmanMatrix();
    void saveImage(std::string a_filename);
    void saveMovie(std::string a_filename);

signals:

public slots:
    void updateFile();
    void updateGPSFile();

private:
    // Left
    DataVisualizer *dataVisualizer;

    // Right layout
    QVBoxLayout *rightLayout;

    // Algorithm
    AlgorithmArea *mAlgorithmArea;

    // Vizualization
    QLabel      *labelVisualizationPart     ;
    QGridLayout *layoutVisualizationPart    ;
    QPushButton *buttonBackward             ;
    QPushButton *buttonStepBackward         ;
    QPushButton *buttonPlay                 ;
    QPushButton *buttonPause                ;
    QPushButton *buttonStepForward          ;
    QPushButton *buttonForward              ;
    QPushButton *buttonFastForward          ;
    QPushButton *buttonFastBackward         ;
    QPushButton **buttonArray               ;

    // Data Handler
    FlDataHandler *flDataHandler;
    FlGPSHandler *flGPSHandler;
    bool fileLoaded;
};

#endif // CENTRALAREA_H
