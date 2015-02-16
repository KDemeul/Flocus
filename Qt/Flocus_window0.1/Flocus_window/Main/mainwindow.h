#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QTreeView>
#include <QTableView>

#include "DataHandling/fldatahandler.h"
#include "Main/centralarea.h"
#include "Main/algorithmarea.h"
#include "common/xmlhandler.h"
#include "Main/extractdialog.h"

class CentralArea;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    QString getFilename();

private slots:
    void openAboutDialogue();
    void openFileDialogue();
    void openGPSDialogue();
    void saveImageDialogue();
    void saveMovieDialogue();

    void openExtractDialogue();

private:
    CentralArea* centralArea;
    QString filename;
    ExtractDialog *mExtractDialog;

};

#endif // MAINWINDOW_H
