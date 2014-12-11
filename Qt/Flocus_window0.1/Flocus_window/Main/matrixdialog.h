#ifndef MATRIXDIALOG_H
#define MATRIXDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

#include "common/common.h"

class MatrixDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MatrixDialog(QWidget *parent = 0);
    void setMatrix(std::vector<cv::Mat> a_matrix);

signals:

public slots:

private:
    QWidget *mMatrixWidget;

};

#endif // MATRIXDIALOG_H
