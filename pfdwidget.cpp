#include "pfdwidget.h"
#include "ui_pfdwidget.h"

#define ToRad(x) (x*0.01745329252)      // *pi/180
#define ToDeg(x) (x*57.2957795131)      // *180/pi

PFDWidget::PFDWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PFDWidget)
{
    ui->setupUi(this);

    QUrl url = QUrl::fromLocalFile("PFD.qml");
    if (!QFile::exists("PFD.qml"))
    {
        QMessageBox::information(0, "Error", "PFD.qml not found!");
        exit(-1);
    }
    m_declarativeView = new QDeclarativeView(url);
    m_declarativeView->setResizeMode(QDeclarativeView::SizeRootObjectToView);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(m_declarativeView);
    setLayout(layout);
    show();
}

PFDWidget::~PFDWidget()
{
    delete ui;
}

void PFDWidget::attitudeChanged(int roll, int pitch, int heading)
{
    QObject *root = m_declarativeView->rootObject();
    root->setProperty("roll", roll);
    root->setProperty("pitch", pitch);
    if (heading<0)
        heading+=360;
    root->setProperty("heading", heading);
}

void PFDWidget::speedChanged(int airspeed, int groundspeed)
{
    QObject *root = m_declarativeView->rootObject();
    root->setProperty("airspeed", airspeed);
    root->setProperty("groundspeed", groundspeed);
}
