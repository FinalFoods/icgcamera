#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <fstream>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <dirent.h>
#include "storage.h"

/*
 * Functions to manage a Design rule for Camera File system (DCF)
 */

Storage::Storage()
{
    // start with storage disabled
    enabled = false;

    // check base directory exists
    if (!dirExists(BASEDIR)) {
        qInfo("ERROR: Storage USB memory not installed or not initialized. (%s not found)", BASEDIR);
        return;
    }

    // check a Digital Camera IMages (DCIM) directory exists
    if (!dirExists(BASEDIR ROOTDIR)){
        qInfo("ERROR: Storage USB memory not installed or not initialized. (%s not found)", (BASEDIR ROOTDIR));
    }
    qInfo("Storage enabled.");
    enabled = true;
}

// scan filenames in a DCF filesystem images directory and return the next filename number for IMG_XXXX.JPG
int Storage::getFileName(char *dirName)
{
    struct dirent **namelist;
    char last[255];
    int n, i;
    int next;

    n = scandir(dirName, &namelist, 0, versionsort);
    if (n < 0) {
        qInfo("ERROR: Reading directory %s.", dirName);
        return -1;
    }
    strcpy(last, namelist[n - 1]->d_name);
    for (i = 0; i < n; i++) {
        //qInfo("getFileName: %s.", namelist[i]->d_name);
        free(namelist[i]);
    }
    free(namelist);

    if (strlen(last) != 12) {
        qInfo("ERROR: Reading directory %s. Largest name not compliant.", dirName);
        return -1;
    }

    next = atoi(last + 4) + 1; // assume 12 characters filenames like IMG_0001.JPG.
    //qInfo("getFileName. last: %s %d", last, next);

    if (next > 9999)
        next = 0;
    return next;
}

// scan directory names in a DCF filesystem top directory and return the largest directory number of XXX_ICG
int Storage::getDirName(char *dirName)
{
    struct dirent **namelist;
    char last[255];
    int n, i;
    int next;

    n = scandir(dirName, &namelist, 0, versionsort);
    if (n < 0) {
        qInfo("ERROR: Reading directory %s.", dirName);
        return -1;
    }
    strcpy(last, namelist[n - 1]->d_name);
    for (i = 0; i < n; i++) {
        //qInfo("getDirName: %s.", namelist[i]->d_name);
        free(namelist[i]);
    }
    free(namelist);

    if (strlen(last) != 7) {
        qInfo("ERROR: Reading top directory %s. Largest name not compliant.", dirName);
        return -1;
    }

    last[3] = '\0'; // terminate name after the three digits.
    next = atoi(last); // assume 7 characters filenames like 100_ICG.
    //qInfo("getDirName. last: %s %d", last, next);

    return next;
}

void Storage::saveImage(void *buf, int bufSize, int width, int height)
{
    char fileName[256];
    char dirName[256];
    void *imbuf = ::operator new (bufSize);
    int ret;

    // figure out the directory name
    ret = getDirName((BASEDIR ROOTDIR));
    if (ret < 0) {
        qInfo("ERROR: Saving the image %s. (ret = %d)", dirName, ret);
        return;
    }
    sprintf(dirName, "%s/%d_ICG", (BASEDIR ROOTDIR), ret);

    // figure out the filename
    ret = getFileName(dirName);
    if (ret < 0) {
        qInfo("ERROR: Saving the image. (ret = %d)", ret);
        return;
    }

    if (ret == 0) {
        // create a new directory and prepare for a new filename
        ret = getDirName((BASEDIR ROOTDIR));
        sprintf(dirName, "%s/%d_ICG", (BASEDIR ROOTDIR), ret + 1);
        mkdir(dirName, 0755);
        ret = 1;  // start new naming for files
    }

    // 12 characters filenames like IMG_0001.JPG
    sprintf(fileName, "%s/IMG_%04d.JPG", dirName, ret);
    qInfo("Saving image in %s", fileName);

    // save the image buffer
    memcpy(imbuf, buf, bufSize);
    // create a QImage and convert to a QPixmap
    QImage qimg = QImage((uchar *)buf, width, height, QImage::Format_RGB888);
    QPixmap qpix = QPixmap(width, height);
    qpix.convertFromImage(qimg);
    // save the QPixmap to a file
    if (!qpix.save(fileName, nullptr, 100)) {
        qInfo("ERROR: Saving the image %s.", fileName);
    }
}


bool Storage::dirExists(const char *path)
{
    struct stat info;

    if (stat(path, &info) != 0)
        return false;

    if (info.st_mode & S_IFDIR)
        return true;

    return false;
}

bool Storage::fileExists(const std::string& fileName)
{
    std::ifstream f(fileName.c_str());
    return f.good();
}

