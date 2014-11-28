#include "mainwindow.h"
#include <QApplication>
#include <QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QStringList arguments = a.arguments();
    arguments.removeFirst();
    QString filePath = arguments.join(" ");
    if (filePath != "")
    {
        w.centerWindow->play(filePath);
    }

    return a.exec();
}
