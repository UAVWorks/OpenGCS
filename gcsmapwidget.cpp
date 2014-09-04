#include "gcsmapwidget.h"
#include "ui_gcsmapwidget.h"

GCSMapWidget::GCSMapWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GCSMapWidget)
{
    map = new mapcontrol::OPMapWidget();
    map->SetZoom(18);

    map->SetShowUAV(true);
    map->UAV->SetUAVPos(internals::PointLatLng(30.604900, 114.352000), 20);
    map->WPCreate(internals::PointLatLng(30.604900, 114.351000), 10, "假想着陆点");
    map->SetShowHome(true);
    map->Home->SetCoord(internals::PointLatLng(30.604600, 114.353000));
    map->SetCurrentPosition(internals::PointLatLng(30.604900, 114.352000));

    ui->setupUi(this);

    ui->mapLayout->addWidget(map);

    QTimer *timer = new QTimer;
    timer->setInterval(100);
    timer->start();

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    map->SetMouseWheelZoomType(internals::MouseWheelZoomType::MousePositionAndCenter);
}

GCSMapWidget::~GCSMapWidget()
{
    delete ui;
    delete map;
}

void GCSMapWidget::update()
{
    internals::PointLatLng ll;

    ll.SetLat(map->currentMousePosition().Lat());
    ll.SetLng(map->currentMousePosition().Lng());
    // map->UAV->SetUAVPos(ll,10);
}

void GCSMapWidget::on_rotateMapLCBtn_clicked()
{
    map->SetRotate(map->Rotate() - 1);
}

void GCSMapWidget::on_centerMapBtn_clicked()
{
    map->SetRotate(0);
}

void GCSMapWidget::on_rotateMapRCBtn_clicked()
{
    map->SetRotate(map->Rotate() + 1);
}

void GCSMapWidget::on_goToBtn_clicked()
{

}

void GCSMapWidget::on_cacheMapBtn_clicked()
{
    cacheVisibleRegion();
}

void GCSMapWidget::cacheVisibleRegion()
{
    internals::RectLatLng rect = map->SelectedArea();

    if (rect.IsEmpty())
    {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("无法缓存离线地图");
        msgBox.setInformativeText("请按住Shift或Alt健，并同时用鼠标左键拖出要缓存的地图区域。");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    else
    {
        map->RipMap();
        // Set empty area = unselect area
        map->SetSelectedArea(internals::RectLatLng());
    }
}
