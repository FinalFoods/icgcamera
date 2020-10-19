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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    int ret;

    ui->setupUi(this);
    // image buffer
    imbuf = ::operator new(Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_BGR));
    if (gpioInitialise() < 0) {
        qInfo("Error in gpioInitialise().");
    }

    buzzer.setPin(21);
/*
    buzzer_pin = 21;

    if (gpioSetMode(buzzer_pin, PI_OUTPUT) != 0) {
        qInfo("in Buzzer::Buzzer. Error in gpioSetMode pin %d", buzzer_pin);
    }
*/
    /*
    buzzerThread = new QThread;
    Buzzer *buzzerObject = new Buzzer;

    buzzerObject->moveToThread(buzzerThread);
    connect(buzzerThread, &QThread::started, buzzerObject, &Buzzer::process);
    connect(buzzerObject, &Buzzer::doneProcess, buzzerThread, &QThread::quit);
    connect(buzzerObject, &Buzzer::showResults, this, &MainWindow::handleResults);
    */
    //connect(buzzerThread, &QThread::finished, buzzerObject, &Buzzer::deleteLater);
    //buzzerThread->start();

    // ----------------------- interrupt setup
    // $ sudo -i
    // # echo "16" > /sys/class/gpio/export
    // # gpio -g mode 16 up

    // ------------------ shutter button setup
    int shutter_button = 16;
    gpioSetMode(shutter_button, PI_INPUT);
    gpioSetAlertFuncEx(shutter_button, _callbackExt, (void *)this);
    debounce = 250;

    // ----------------------- camera setup
    Camera.setVerticalFlip(true);
    //Open camera
    qInfo(" Opening Camera...");
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

    qInfo("in_callback. starting buzzer.");
    buzzer.play();

/*
    if ((ret = gpioWrite(buzzer_pin, 1)) != 0) {
        qInfo("in Buzzer::process. Error writing pin %d (1)", ret);
    }

    gpioSleep(PI_TIME_RELATIVE, 1, 100000); // sleep for 1.1 sec
    if ((ret = gpioWrite(buzzer_pin, 0)) != 0) {
        qInfo("in Buzzer::process. Error writing pin %d (1)", ret);
    }
*/
    //buzzerThread->start();
}

void MainWindow::_callbackExt(int gpio, int level, uint32_t tick, void *user)
{
    MainWindow *mySelf = (MainWindow *) user;

    auto time_now = std::chrono::high_resolution_clock::now();
    std::chrono::duration< double > delta = time_now - (mySelf->last_time);
    std::chrono::milliseconds d = std::chrono::duration_cast < std::chrono::milliseconds >(delta);
    // qInfo("in_callbackExt. %d", d.count());
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
    // Camera.retrieve((unsigned char *)imbuf, raspicam::RASPICAM_FORMAT_BGR);
    Camera.retrieve((unsigned char *)imbuf, raspicam::RASPICAM_FORMAT_IGNORE);

/*
    // obtain a Mat image from the capture
    Mat M(Camera.getHeight(), Camera.getWidth(), CV_8UC3, (void *)imbuf);
    // convert to QImage
    QImage qimg = QImage((uchar *)M.data, M.cols, M.rows, M.step,  QImage::Format_RGB888);
*/
    QImage qimg = QImage((uchar *)imbuf, Camera.getWidth(), Camera.getHeight(), QImage::Format_RGB888);
    painter.drawImage(QPoint(0, 0), qimg);
}
