#ifndef PFDWIDGET_H
#define PFDWIDGET_H

#include <QUrl>
#include <QFile>
#include <QTimer>
#include <QWidget>
#include <QSlider>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QGraphicsObject>
#include <QDeclarativeView>

namespace Ui {
class PFDWidget;
}

class PFDWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PFDWidget(QWidget *parent = 0);
    ~PFDWidget();

private slots:
    void attitudeChanged(int roll, int pitch, int heading);
    void speedChanged(int airspeed, int groundspeed);

private:
    Ui::PFDWidget *ui;
    QDeclarativeView* m_declarativeView;
};

#endif // PFDWIDGET_H
