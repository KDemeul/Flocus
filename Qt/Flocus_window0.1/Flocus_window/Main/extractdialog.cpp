#include "extractdialog.h"

ExtractDialog::ExtractDialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *layoutMsg = new QVBoxLayout;

    QString title("Extract infos");
    setWindowTitle(title);

    QLabel *label = new QLabel;
    QString textLabel = "<b> Extract statistics from different algorithm.<b>";
    label->setText(textLabel);
    layoutMsg->addWidget(label);

    mStandardItemModel = XMLhandler::getItemModel();

    view = new QTableView;
    view->setModel(mStandardItemModel);
    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    layoutMsg->addWidget(view);

    QWidget *bottom = new QWidget();
    QHBoxLayout *layoutBottom = new QHBoxLayout;

    QPushButton *buttonStat = new QPushButton("Statistics");
    layoutBottom->addWidget(buttonStat,0,Qt::AlignCenter);

    QPushButton *buttonOK = new QPushButton("OK");
    buttonOK->setMaximumWidth(50);
    layoutBottom->addWidget(buttonOK,0,Qt::AlignCenter);
    bottom->setLayout(layoutBottom);

    layoutMsg->addWidget(bottom);

    connect(buttonStat,SIGNAL(clicked()),this,SLOT(displayStat()));
    connect(buttonOK,SIGNAL(clicked()),this,SLOT(accept()));

    this->setLayout(layoutMsg);
}

void ExtractDialog::displayStat()
{
    QItemSelectionModel *selection = view->selectionModel();
    QModelIndexList listeSelections = selection->selectedIndexes();
    std::vector<double> values;
    for (int i = 0 ; i < listeSelections.size() ; i++)
    {
        QVariant elementSelectionne = mStandardItemModel->data(listeSelections[i], Qt::DisplayRole);
        values.push_back(elementSelectionne.toDouble());
    }

    double mean = 0;
    int size = 0;
    for(std::vector<double>::iterator it = values.begin(); it != values.end(); it++)
    {
        if(*it != 0)
        {
            mean += *it;
            size ++;
        }
    }
    mean /= size;

    double var=0;
    for(std::vector<double>::iterator it = values.begin(); it != values.end(); it++)
    {
        var  += pow(*it - mean,2);
    }
    var /= values.size();
    var = sqrt(var);

    std::stringstream ss;
    ss << "Mean: " << mean << " Var: " << var;
    QMessageBox::information(this, "Statistics", ss.str().c_str());
}
