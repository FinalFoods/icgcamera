#include <QTimer>
#include <QApplication>
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTimer timer;

    w.show();

    QObject::connect(&timer, &QTimer::timeout, [&]()
    {
        w.update();
    });
    timer.start(33);

    return a.exec();
}
