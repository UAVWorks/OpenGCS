#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->modeBox->setMaximumHeight(60);
    ui->menuBar->setVisible(false);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(ui->modeBox);
    mainLayout->addWidget(ui->stackedWidget);
    ui->centralWidget->setLayout(mainLayout);

    QFile styleSheet(":files/styles/style-dark.css");
    if (styleSheet.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qApp->setOverrideCursor(Qt::WaitCursor);
        qApp->setStyleSheet(styleSheet.readAll());
        qApp->restoreOverrideCursor();
    }

    mapWidget = new GCSMapWidget();
    pfdWidget_1 = new PFDWidget();
    pfdWidget_2 = new PFDWidget();
    cameraWidget = new CameraWidget();
    cameraWidget->connectToCamera();
    jsWidget = new JSWidget();

    QLabel *infoLabel = new QLabel("无人机编号:\n02\n\n位置坐标:\n30.604900\n114.352000\n\n海拔高度:\n20 m\n\n姿态角:\n0, 0, 0\n");
    infoLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    QVBoxLayout *page1_VLayout_1 = new QVBoxLayout();    
    QHBoxLayout *page1_HLayout_2 = new QHBoxLayout();

    page1_HLayout_2->addWidget(cameraWidget, 3);
    page1_HLayout_2->addWidget(infoLabel, 1);

    page1_VLayout_1->addLayout(page1_HLayout_2);
    page1_VLayout_1->addWidget(pfdWidget_1);

    ui->page1_HLayout_1->addLayout(page1_VLayout_1, 3);
    ui->page1_HLayout_1->addWidget(mapWidget, 5);
    ui->page1_HLayout_1->setSpacing(0);

    ui->page2_HLayout_1->addWidget(pfdWidget_2, 1);
    ui->page2_HLayout_1->insertSpacing(1, 50);
    ui->page2_HLayout_1->addWidget(jsWidget, 2);

//    setDockOptions(AnimatedDocks | AllowTabbedDocks | AllowNestedDocks);
//    setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);

    ui->radioButton_1->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->radioButton_1, SIGNAL(clicked()), this, SLOT(setPathPlanningMode()));
    connect(ui->radioButton_2, SIGNAL(clicked()), this, SLOT(setJoyStickCtrlMode()));
    connect(ui->radioButton_3, SIGNAL(clicked()), this, SLOT(setDataPlottingMode()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPathPlanningMode()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->spToolBar->setVisible(true);
}

void MainWindow::setJoyStickCtrlMode()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->spToolBar->setVisible(false);
}

void MainWindow::setDataPlottingMode()
{
    ui->stackedWidget->setCurrentIndex(2);
}
