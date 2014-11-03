#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
//#include <QtWidgets>
#include "centralarea.h"
#include "fldatahandler.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private slots:
    void openAboutDialogue();
    void openFileDialogue();

private:
    CentralArea *centralArea;
    QString filename;
    FlDataHandler *flDataHandler;

};

#endif // MAINWINDOW_H
