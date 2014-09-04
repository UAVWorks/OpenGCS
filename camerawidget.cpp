#include "camerawidget.h"
#include "ui_camerawidget.h"
#include <QMessageBox>

CameraWidget::CameraWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraWidget)
{
    ui->setupUi(this);
    imageBuffer = new ImageBuffer(100);
    cameraCapture = new CameraCapture(imageBuffer, 0);
    imageProcessing = new ImageProcessing(imageBuffer, cameraCapture->getInputSourceWidth(), cameraCapture->getInputSourceHeight());

}

CameraWidget::~CameraWidget()
{
    delete ui;
}

void CameraWidget::connectToCamera()
{
    if(cameraCapture->isCameraConnected())
    {
        connect(imageProcessing, SIGNAL(newFrame(QImage)), this, SLOT(updateFrame(QImage)), Qt::QueuedConnection);
        cameraCapture->start(QThread::IdlePriority);
        imageProcessing->start();
    }
    else
        QMessageBox::warning(this,"ERROR:","Could not connect to camera.");
}

void CameraWidget::disconnectCamera()
{
    disconnect(imageProcessing, SIGNAL(newFrame(QImage)), this, SLOT(updateFrame(QImage)));

    ui->frameLabel->setText("No Camera Connected!");

    if(cameraCapture->isRunning())
    {
        cameraCapture->stopCaptureThread();
    }
    if(imageProcessing->isRunning())
    {
        imageProcessing->stopProcessingThread();
    }
    imageBuffer->clearBuffer();

    if(cameraCapture->isFinished()&&imageProcessing->isFinished())
    {
        if(cameraCapture->isCameraConnected())
        {
            cameraCapture->disconnectCamera();
        }

        delete imageBuffer;
        delete cameraCapture;
        delete imageProcessing;
    }
}

void CameraWidget::updateFrame(const QImage &frame)
{
    ui->frameLabel->setPixmap(QPixmap::fromImage(frame));
}
