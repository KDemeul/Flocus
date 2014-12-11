#include "matrixdialog.h"

MatrixDialog::MatrixDialog(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *layoutMsg = new QVBoxLayout;

    QString title("Kalman Matrix");
    setWindowTitle(title);

    QLabel *label = new QLabel;
    QString textLabel = "<b>A, B, Q, C, R, mu, Sigma<b>";
    label->setText(textLabel);
    layoutMsg->addWidget(label);

    mMatrixWidget = new QWidget;
    layoutMsg->addWidget(mMatrixWidget);

    QPushButton *buttonOK = new QPushButton("OK");
    buttonOK->setMaximumWidth(50);
    layoutMsg->addWidget(buttonOK);

    connect(buttonOK,SIGNAL(clicked()),this,SLOT(accept()));

    this->setLayout(layoutMsg);
}

void MatrixDialog::setMatrix(std::vector<cv::Mat> a_matrix)
{
    QHBoxLayout *layout = new QHBoxLayout();
    mMatrixWidget->setLayout(layout);

    // Add vertical line
    QFrame *line = new QFrame();    // <<< this does the trick
    line->setLineWidth(2);
    line->setMidLineWidth(1);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Raised);
    layout->addWidget(line);

    for(std::vector<cv::Mat>::iterator it = a_matrix.begin(); it != a_matrix.end(); it++){
        cv::Mat currentMat = *it;
        QWidget *matrix = new QWidget;
        QGridLayout *matrixLayout = new QGridLayout;
        matrix->setLayout(matrixLayout);

        for(int i=0; i < currentMat.rows ; i++){
            for(int j=0 ; j < currentMat.cols ; j++){
                std::stringstream ss;
                ss << currentMat.at<double>(i,j);
                std::string tmp;
                ss >> tmp;
                QLabel *cell = new QLabel();
                cell->setText(tmp.c_str());
                matrixLayout->addWidget(cell,i,j);
            }
        }

        layout->addWidget(matrix);

        // Add vertical line
        QFrame *line = new QFrame();    // <<< this does the trick
        line->setLineWidth(2);
        line->setMidLineWidth(1);
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Raised);
        layout->addWidget(line);
    }
}
