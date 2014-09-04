#ifndef GCSMAPWIDGET_H
#define GCSMAPWIDGET_H

#include <QWidget>
#include "mapwidget/opmapwidget.h"
#include "mapwidget/waypointitem.h"

namespace Ui {
class GCSMapWidget;
}

class GCSMapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GCSMapWidget(QWidget *parent = 0);
    ~GCSMapWidget();
    void cacheVisibleRegion();

private:
    Ui::GCSMapWidget *ui;
    mapcontrol::OPMapWidget *map;

private slots:
    void update();
    void on_goToBtn_clicked();
    void on_cacheMapBtn_clicked();
    void on_rotateMapLCBtn_clicked();
    void on_rotateMapRCBtn_clicked();
    void on_centerMapBtn_clicked();
};

#endif // GCSMAPWIDGET_H
