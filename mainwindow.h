#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPainter>
#include <raspicam/raspicam.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    raspicam::RaspiCam Camera;
    void *imbuf;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent* event);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
