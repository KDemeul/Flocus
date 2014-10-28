#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon("../../fig/flocus_small_icon.png"));
    w.showMaximized();

    return a.exec();
}
