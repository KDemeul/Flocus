#ifndef CENTRALAREA_H
#define CENTRALAREA_H

#include <QWidget>
#include <QtWidgets>
#include "datavisualizer.h"

class CentralArea : public QWidget
{
    Q_OBJECT

public:
    CentralArea(QWidget *parent = 0);
    QCheckBox* getButtonRANSAC();
    QCheckBox* getButtonKALMAN();

signals:

public slots:

private:
    // Left
    DataVisualizer *dataVisualizer;

    // Algorithm
    QLabel      *labelAlgorithmPart;
    QCheckBox   *buttonRANSAC      ;
    QCheckBox   *buttonKALMAN      ;

    // Vizualization
    QLabel      *labelVisualizationPart     ;
    QGridLayout  *layoutVisualizationPart    ;
    QPushButton *buttonBackward             ;
    QPushButton *buttonStepBackward         ;
    QPushButton *buttonPlay                 ;
    QPushButton *buttonPause                ;
    QPushButton *buttonStepForward          ;
    QPushButton *buttonForward              ;
};

#endif // CENTRALAREA_H
