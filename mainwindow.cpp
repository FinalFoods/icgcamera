#include <QtGui/QImage>
#include <QDebug>
#include <QThread>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

#include <ctime>
#include <unistd.h>

#include <raspicam/raspicam.h>
#include <pigpio/pigpio.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "buzzer.h"

using namespace cv;

#define BUZZER_PIN 21
#define SHUTTER_PIN 16

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    int ret;

    ui->setupUi(this);
    // -- image buffer setup
    imSize = Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_BGR);
    imbuf = ::operator new(imSize);

    // -- initialize pigpio library
    if (gpioInitialise() < 0) {
        qInfo("Error in gpioInitialise().");
    }

    // -- buzzer setup
    buzzer.setPin(BUZZER_PIN);

    // ------------------ shutter button setup
    gpioSetMode(SHUTTER_PIN, PI_INPUT);
    gpioSetPullUpDown(SHUTTER_PIN, PI_PUD_UP);
    gpioSetAlertFuncEx(SHUTTER_PIN, _callbackExt, (void *)this);
    debounce = 250;

    // ----------------------- camera setup
    Camera.setVerticalFlip(true);
    //Open camera
    qInfo("Opening Camera...");
    if (!Camera.open()) {
        qInfo("Error opening camera");
        return;
    }
    // wait until the camera stabilizes
    qInfo("Camera width, height: %d %d", Camera.getWidth(), Camera.getHeight());
    qInfo("Sleeping for 3 secs");
    usleep(3000000);
    qInfo("Camera ready.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleResults(int res) {
    qInfo("in handleResults.");
}

void MainWindow::_callback(int gpio, int level, uint32_t tick)
{
    int ret;

    if (buzzer.isPlaying())
            return;

    qInfo("in_callback. starting buzzer and save image.");
    buzzer.play();
    storage.saveImage(imbuf, imSize, Camera.getWidth(), Camera.getHeight());
}

void MainWindow::_callbackExt(int gpio, int level, uint32_t tick, void *user)
{
    MainWindow *mySelf = (MainWindow *) user;

    auto time_now = std::chrono::high_resolution_clock::now();
    std::chrono::duration< double > delta = time_now - (mySelf->last_time);
    std::chrono::milliseconds d = std::chrono::duration_cast < std::chrono::milliseconds >(delta);

    mySelf->last_time = time_now;
    if (d.count() < mySelf->debounce)
        return;

    mySelf->_callback(gpio, level, tick);
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    Camera.grab();
    // extract the image
    Camera.retrieve((unsigned char *)imbuf, raspicam::RASPICAM_FORMAT_IGNORE);  // RASPICAM_FORMAT_BGR

/*
    // obtain a Mat image from the capture
    Mat M(Camera.getHeight(), Camera.getWidth(), CV_8UC3, (void *)imbuf);
    // convert to QImage
    QImage qimg = QImage((uchar *)M.data, M.cols, M.rows, M.step,  QImage::Format_RGB888);
*/
    QImage qimg = QImage((uchar *)imbuf, Camera.getWidth(), Camera.getHeight(), QImage::Format_RGB888);
    painter.drawImage(QPoint(0, 0), qimg);
}
