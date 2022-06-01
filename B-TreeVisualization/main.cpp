#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile themeFile("../Theme/Diplaytap.qss");
    themeFile.open(QFile::ReadOnly);
    QString theme = QLatin1String(themeFile.readAll());
    a.setStyleSheet(theme);
    MainWindow w;
    w.show();
    return a.exec();
}
