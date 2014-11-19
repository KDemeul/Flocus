#include <QApplication>
#include <QFile>
#include "Main/mainwindow.h"
#include "common/xmlhandler.h"

int main(int argc, char *argv[])
{
    XMLhandler::init();

    QApplication::setFont(QFont("Helvetica"));
    QApplication a(argc, argv);
    QFile File("stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    a.setStyleSheet(StyleSheet);

    MainWindow w;
    w.setMinimumSize(1000,600);
    w.setWindowIcon(QIcon("../../fig/flocus_small_icon.png"));
    w.show();

    return a.exec();
}
