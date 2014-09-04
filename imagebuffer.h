#ifndef IMAGEBUFFER_H
#define IMAGEBUFFER_H

#include <QWaitCondition>
#include <QMutex>
#include <QQueue>
#include <QSemaphore>
#include <QDebug>
#include <opencv/highgui.h>

class ImageBuffer
{

public:
    ImageBuffer(int size);
    void addFrame(const IplImage *image);
    IplImage* getFrame();
    void clearBuffer();
    int getSizeOfImageBuffer();

private:
    QMutex mutex;
    QQueue<IplImage*> imageQueue;
    QSemaphore *freeSlots;
    QSemaphore *usedSlots;
    QSemaphore *clearBuffer1;
    QSemaphore *clearBuffer2;
    int bufferSize;
};


#endif // IMAGEBUFFER_H
