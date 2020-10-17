#include <QtGui/QImage>
#include <QDebug>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

#include <ctime>
#include <unistd.h>

#include <raspicam/raspicam.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // image buffer
    imbuf = ::operator new(Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_BGR));

    Camera.setVerticalFlip(true);
    //Open camera
    qInfo(" Opening Camera...");
    if (!Camera.open()) {
        qInfo("Error opening camera");
        return;
    }

    //wait a while until camera stabilizes
    qInfo("Camera width, height: %d %d", Camera.getWidth(), Camera.getHeight());
    qInfo("Sleeping for 3 secs");
    usleep(3000000);
    qInfo("Camera ready.");
}

MainWindow::~MainWindow()
{
    delete ui;
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
