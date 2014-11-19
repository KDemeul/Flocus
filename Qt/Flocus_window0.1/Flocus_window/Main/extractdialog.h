#ifndef EXTRACTDIALOG_H
#define EXTRACTDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLayout>
#include <QLabel>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QPushButton>
#include <QTableView>
#include <QMessageBox>

#include "common/common.h"
#include "common/xmlhandler.h"

class ExtractDialog : public QDialog
{
    Q_OBJECT
public:
    ExtractDialog(QWidget *parent = 0);

private slots:
    void displayStat();

private:
    QStandardItemModel *mStandardItemModel;
    QTableView *view;
};

#endif // EXTRACTDIALOG_H
