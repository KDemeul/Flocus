#include "datavisualizer.h"

DataVisualizer::DataVisualizer(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumSize(800,800);
    this->setStyleSheet("background-color:white");

    QLabel *label = new QLabel("Data Visualizer",this);
    label->setAlignment(Qt::AlignCenter);
}
