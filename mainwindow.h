#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPainter>
#include <raspicam/raspicam.h>

#include "buzzer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    raspicam::RaspiCam Camera;
    void *imbuf;
    Buzzer buzzer;


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent* event);

private:
    Ui::MainWindow *ui;

    void _callback(int, int, uint32_t);
    static void _callbackExt(int, int, uint32_t, void*);
    std::chrono::high_resolution_clock::time_point last_time;
    int debounce;

public slots:
    void handleResults(int res);
};

#endif // MAINWINDOW_H
