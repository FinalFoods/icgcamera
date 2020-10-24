#ifndef STORAGE_H
#define STORAGE_H


#define BASEDIR "/media/icgcamera"
#define ROOTDIR "/DCIM"

class Storage
{
    bool enabled;

    int getFileName(char *dirName);
    int getDirName(char *dirName);

    char videoDirName[256];
    int imgCounter;
public:
    Storage();
    bool dirExists(const char *path);
    bool fileExists(const std::string& fileName);
    void saveImage(void *buf, int bufSize, int width, int height);

    void prepareRecording(void);
    void finalizeRecording(void);
    int saveFrame(const char *buf, int bufSize, int width, int height);
};

#endif // STORAGE_H
