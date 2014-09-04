#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include "imagebuffer.h"
#include "cameracapture.h"
#include "imageprocessing.h"

namespace Ui {
class CameraWidget;
}

class CameraWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CameraWidget(QWidget *parent = 0);
    ~CameraWidget();

public slots:
    void connectToCamera();
    void disconnectCamera();

private slots:
    void updateFrame(const QImage &frame);

private:
    Ui::CameraWidget *ui;
    ImageBuffer* imageBuffer;
    CameraCapture* cameraCapture;
    ImageProcessing* imageProcessing;
    int sourceWidth;
    int sourceHeight;
    int deviceNumber;
    int imageBufferSize;
};

#endif // CAMERAWIDGET_H
