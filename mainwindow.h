#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPainter>
#include <raspicam/raspicam.h>

#include "shutter.h"
#include "storage.h"
#include "irleds.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    raspicam::RaspiCam Camera;
    int imSize;
    void *imbuf;
    int cameraWidth, cameraHeight;

    Shutter shutter;
    Storage storage;
    IRleds irleds;
    std::chrono::high_resolution_clock::time_point last_time;
    int debounce;
    bool recording;
    int recRate;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent* event);

private:
    Ui::MainWindow *ui;

    void _callback(int, int, uint32_t);
    static void _buttonsCallback(int, int, uint32_t, void*);

};

#endif // MAINWINDOW_H
