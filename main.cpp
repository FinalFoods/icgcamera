#include <QTimer>
#include <QApplication>
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTimer timer;

    // use the main window full screen as camera viewer
    w.setWindowState(Qt::WindowFullScreen);
    w.show();

    // timer loop to refresh the camera image on the main window
    QObject::connect(&timer, &QTimer::timeout, [&]()
        {
            w.update();
        });

    // refresh at 30 frames per second
    timer.start(33);

    return a.exec();
}
