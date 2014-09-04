#ifndef JSWIDGET_H
#define JSWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QDateTime>
#include <QWidget>
#include <QDebug>
#include <QTimer>

#include "joystick.h"
#include "ctrlcmd.h"
#include "qextserialport.h"
#include "qextserialenumerator.h"

namespace Ui {
class JSWidget;
}

class QextSerialPort;
class QextSerialEnumerator;

class JSWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JSWidget(QWidget *parent = 0);
    ~JSWidget();
    CtrlCmd *mCmd;
    int AddEvenParity(int value, int bits);

public slots:
    void onSendData();
    void onJoysChanged(int count, QListIterator<Joystick *> i);
    void onDataChanged(int count, QListIterator<Joystick *> i);
    void onjsScanButtonClicked();
    void onjsStartButtonClicked();
    void onjsStopButtonClicked();

private Q_SLOTS:
    void onPortNameChanged(const QString &name);
    void onBaudRateChanged(int idx);
    void onParityChanged(int idx);
    void onDataBitsChanged(int idx);
    void onStopBitsChanged(int idx);
    void onspStartButtonClicked();
    void onspStopButtonClicked();
    void onReadyRead();
    void onClearBtnClicked();
    void onSaveBtnClicked();

signals:
    void scan();
    void start(int timeout);
    void stop();

private:
    Ui::JSWidget *ui;
    QTimer *sendTimer;
    QextSerialPort *port;
    QextSerialEnumerator *enumerator;
};

#endif // JSWIDGET_H
