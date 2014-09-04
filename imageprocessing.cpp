#include "imageprocessing.h"

ImageProcessing::ImageProcessing(ImageBuffer *imageBuffer, int inputSourceWidth, int inputSourceHeight)
                                   : QThread(), imageBuffer(imageBuffer), inputSourceWidth(inputSourceWidth),
                                   inputSourceHeight(inputSourceHeight)
{
    currentFrameCopy=cvCreateImage(cvSize(inputSourceWidth,inputSourceHeight),IPL_DEPTH_8U,3);
    stopped=false;
}

ImageProcessing::~ImageProcessing()
{
    if(currentFrameCopy!=NULL)
    {
        cvReleaseImage(&currentFrameCopy);
    }
}

void ImageProcessing::run()
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

        IplImage* currentFrame = imageBuffer->getFrame();
        if(currentFrame!=NULL)
        {
            cvCopy(currentFrame,currentFrameCopy);
///////////////////////////////////////////////////////////////////////////////////////////////////
            updateMembersMutex.lock();
            frame=IplImageToQImage(currentFrameCopy);
            updateMembersMutex.unlock();
///////////////////////////////////////////////////////////////////////////////////////////////////
            currentSizeOfBuffer=imageBuffer->getSizeOfImageBuffer();
            emit newFrame(frame);
            if(currentFrame!=NULL)
            {
                cvReleaseImage(&currentFrame);
            }
        }
        else
            qDebug() << "ERROR: Processing thread received a NULL image.";
    }
    qDebug() << "Stopping processing thread...";
}

void ImageProcessing::stopProcessingThread()
{
    stoppedMutex.lock();
    stopped=true;
    stoppedMutex.unlock();
}

int ImageProcessing::getCurrentSizeOfBuffer()
{
    return currentSizeOfBuffer;
}

QImage ImageProcessing::IplImageToQImage(const IplImage *iplImage)
{
    int height = iplImage->height;
    int width = iplImage->width;
    if(iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 1)
    {
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
        QImage img(qImageBuffer, width, height, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    else if(iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 3)
    {
        const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
        QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qDebug() << "ERROR: IplImage could not be converted to QImage.";
        return QImage();
    }
}
