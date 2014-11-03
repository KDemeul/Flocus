#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setFont(QFont("Helvetica"));
    QApplication a(argc, argv);
    MainWindow w;
    w.setMinimumSize(800,600);
    w.setWindowIcon(QIcon("../../fig/flocus_small_icon.png"));
    w.show();

    return a.exec();
}
