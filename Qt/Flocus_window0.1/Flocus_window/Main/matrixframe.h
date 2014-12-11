#ifndef MATRIXFRAME_H
#define MATRIXFRAME_H

#include <QObject>
#include <QDialog>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

#include "common/common.h"

class MatrixFrame : public QFrame
{
    Q_OBJECT
public:
    explicit MatrixFrame(QWidget *parent = 0);

signals:

public slots:
    void setMatrix(std::vector<cv::Mat> a_matrix);

private:
    QWidget *mMatrixWidget;
    QGridLayout *mMatrixLayout;

    QLabel *m_A;
    QLabel *m_B;
    QLabel *m_Q;
    QLabel *m_C;
    QLabel *m_R;
    QLabel *m_mu;
    QLabel *m_Sigma;

};

#endif // MATRIXFRAME_H
