#include "cameracapture.h"

CameraCapture::CameraCapture(ImageBuffer *buffer, int deviceNumber) : QThread(), imageBuffer(buffer)
{
    capture=cvCaptureFromCAM(deviceNumber);
    stopped=false;
}

void CameraCapture::run()
{
    while(1)
    {
        stoppedMutex.lock();
        if (stopped)
        {
            stopped=false;
            stoppedMutex.unlock();
            break;
        }
        stoppedMutex.unlock();
        imageBuffer->addFrame(cvQueryFrame(capture));
    }
    qDebug() << "Stopping capture thread...";
}

void CameraCapture::disconnectCamera()
{
    if(capture!=NULL)
    {
        cvReleaseCapture(&capture);
        if(capture==NULL)
            qDebug() << "Camera successfully disconnected.";
        else
            qDebug() << "ERROR: Camera could not be disconnected.";
    }
}

void CameraCapture::stopCaptureThread()
{
    stoppedMutex.lock();
    stopped=true;
    stoppedMutex.unlock();
}

bool CameraCapture::isCameraConnected()
{
    if(capture!=NULL)
        return true;
    else
        return false;
}

int CameraCapture::getInputSourceWidth()
{
    return cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
}

int CameraCapture::getInputSourceHeight()
{
    return cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
}
