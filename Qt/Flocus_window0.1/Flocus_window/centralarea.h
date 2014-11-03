#ifndef CENTRALAREA_H
#define CENTRALAREA_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>

#include "datavisualizer.h"
#include "fldatahandler.h"

class CentralArea : public QWidget
{
    Q_OBJECT

public:
    CentralArea(QWidget *parent = 0);
    QCheckBox* getButtonRANSAC();
    QCheckBox* getButtonKALMAN();

signals:

public slots:
    void updateImage();

private:
    // Left
    DataVisualizer *dataVisualizer;

    // Algorithm
    QLabel      *labelAlgorithmPart;
    QCheckBox   *buttonRANSAC      ;
    QCheckBox   *buttonKALMAN      ;

    // Vizualization
    QLabel      *labelVisualizationPart     ;
    QGridLayout *layoutVisualizationPart    ;
    QPushButton *buttonBackward             ;
    QPushButton *buttonStepBackward         ;
    QPushButton *buttonPlay                 ;
    QPushButton *buttonPause                ;
    QPushButton *buttonStepForward          ;
    QPushButton *buttonForward              ;

    // Data Handler
    FlDataHandler *flDataHandler;
};

#endif // CENTRALAREA_H
