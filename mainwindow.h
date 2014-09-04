#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QMainWindow>
#include "gcsmapwidget.h"
#include "pfdwidget.h"
#include "camerawidget.h"
#include "jswidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GCSMapWidget *mapWidget;
    PFDWidget *pfdWidget_1;
    PFDWidget *pfdWidget_2;
    CameraWidget *cameraWidget;
    JSWidget *jsWidget;

private slots:
    void setPathPlanningMode();
    void setJoyStickCtrlMode();
    void setDataPlottingMode();
};

#endif // MAINWINDOW_H
