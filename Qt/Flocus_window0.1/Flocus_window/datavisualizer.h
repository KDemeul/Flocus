#ifndef DATAVISUALIZER_H
#define DATAVISUALIZER_H

#include <QWidget>
#include <QLabel>
#include <QVTKWidget.h>
//#include <QtWidgets>

class DataVisualizer : public QVTKWidget
{
    Q_OBJECT
public:
    explicit DataVisualizer(QWidget *parent = 0);

signals:

public slots:

};

#endif // DATAVISUALIZER_H
