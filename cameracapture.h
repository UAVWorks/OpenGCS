#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <QtGui>
#include <QDebug>
#include <QThread>
#include <opencv/highgui.h>
#include "imagebuffer.h"

class CameraCapture : public QThread
{
    Q_OBJECT

public:
    CameraCapture(ImageBuffer *buffer, int deviceNumber);
    void disconnectCamera();
    void stopCaptureThread();
    bool isCameraConnected();
    int getInputSourceWidth();
    int getInputSourceHeight();

private:
    CvCapture *capture;
    ImageBuffer *imageBuffer;
    QMutex stoppedMutex;
    volatile bool stopped;

protected:
    void run();
};

#endif // CAMERACAPTURE_H
