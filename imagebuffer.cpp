#include "imagebuffer.h"

ImageBuffer::ImageBuffer(int bufferSize) : bufferSize(bufferSize)
{
    freeSlots = new QSemaphore(bufferSize);
    usedSlots = new QSemaphore(0);
    clearBuffer1 = new QSemaphore(1);
    clearBuffer2 = new QSemaphore(1);
}

void ImageBuffer::addFrame(const IplImage* image)
{
    clearBuffer1->acquire();
    freeSlots->acquire();
    IplImage* temp = cvCloneImage(image);
    mutex.lock();
    imageQueue.enqueue(temp);
    mutex.unlock();
    clearBuffer1->release();
    usedSlots->release();
}

IplImage* ImageBuffer::getFrame()
{
    clearBuffer2->acquire();
    usedSlots->acquire();
    IplImage* temp=0;
    mutex.lock();
    temp=imageQueue.dequeue();
    mutex.unlock();
    freeSlots->release();
    clearBuffer2->release();
    return temp;
}

void ImageBuffer::clearBuffer()
{
    if(imageQueue.size()!=0)
    {
        clearBuffer1->acquire();
        clearBuffer2->acquire();
        freeSlots->release(imageQueue.size());
        freeSlots->acquire(bufferSize);
        usedSlots->acquire(imageQueue.size());
        while(imageQueue.size()!=0)
        {
            IplImage* temp;
            temp=imageQueue.dequeue();
            cvReleaseImage(&temp);
        }
        freeSlots->release(bufferSize);
        clearBuffer2->release();
        clearBuffer1->release();
        qDebug() << "Image buffer successfully cleared.";
    }
    else
        qDebug() << "WARNING: Could not clear image buffer: already empty.";
}

int ImageBuffer::getSizeOfImageBuffer()
{
    return imageQueue.size();
}

