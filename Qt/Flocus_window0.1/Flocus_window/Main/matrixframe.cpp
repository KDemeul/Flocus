#include "matrixframe.h"

MatrixFrame::MatrixFrame(QWidget *parent)
{
    QVBoxLayout *layoutMsg = new QVBoxLayout;
    layoutMsg->setAlignment(Qt::AlignCenter);

    QString title("Kalman Matrix");
    setWindowTitle(title);

    QLabel *label = new QLabel;
    QString textLabel = "<b>A, B, Q, C, R, mu, Sigma<b>";
    label->setText(textLabel);
    layoutMsg->addWidget(label);

    mMatrixWidget = new QWidget;
    layoutMsg->addWidget(mMatrixWidget);
    mMatrixLayout = new QGridLayout();
    mMatrixWidget->setLayout(mMatrixLayout);

    // Add each label to its correct pos
    m_A = new QLabel();

    QPushButton *buttonOK = new QPushButton("OK");
    buttonOK->setMaximumWidth(50);
    layoutMsg->addWidget(buttonOK);

    connect(buttonOK,SIGNAL(clicked()),this,SLOT(close()));

    this->setLayout(layoutMsg);
}

void MatrixFrame::setMatrix(std::vector<cv::Mat> a_matrix)
{
    int currentRow = 0;
    int currentCol = 0;
    for(std::vector<cv::Mat>::iterator it = a_matrix.begin(); it != a_matrix.end(); it++){
//        // Delete previous matrix
//        if(mMatrixLayout->itemAtPosition(currentRow,currentCol) != NULL){
//            QWidget* widget = mMatrixLayout->itemAtPosition(currentRow,currentCol)->widget();
//            delete widget;
//        }

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

        mMatrixLayout->addWidget(matrix,currentRow,currentCol);
        currentCol = currentCol > 2 ? 0 : currentCol+1;
        currentRow = currentCol == 0 ? currentRow+1:  currentRow;
    }
}

//        // Add vertical line
//        QFrame *line = new QFrame();
//        line->setLineWidth(2);
//        line->setMidLineWidth(1);
//        line->setFrameShape(QFrame::VLine);
//        line->setFrameShadow(QFrame::Raised);
//        layout->addWidget(line);
