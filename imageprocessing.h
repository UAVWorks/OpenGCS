#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QThread>
#include <QtGui>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "imagebuffer.h"

class ImageProcessing : public QThread
{
    Q_OBJECT

public:
    ImageProcessing(ImageBuffer *imageBuffer, int inputSourceWidth, int inputSourceHeight);
    ~ImageProcessing();
    QImage IplImageToQImage(const IplImage *iplImage);
    void stopProcessingThread();
    int getCurrentSizeOfBuffer();

private:
    ImageBuffer *imageBuffer;
    volatile bool stopped;
    int inputSourceWidth;
    int inputSourceHeight;
    int currentSizeOfBuffer;
    IplImage *currentFrameCopy;
    QImage frame;
    QMutex stoppedMutex;
    QMutex updateMembersMutex;

protected:
    void run();

signals:
    void newFrame(const QImage &frame);
};

#endif // IMAGEPROCESSING_H
