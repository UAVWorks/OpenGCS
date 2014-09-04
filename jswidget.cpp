#include "jswidget.h"
#include "ui_jswidget.h"

JSWidget::JSWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JSWidget)
{
    ui->setupUi(this);

    mCmd = new CtrlCmd(0, 0, 0, 0, 0, 0);
    sendTimer = new QTimer;

    foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
        ui->portBox->addItem(info.portName);
    //ui->portBox->setEditable(true);

    ui->baudRateBox->addItem("9600", BAUD9600);
    ui->baudRateBox->addItem("19200", BAUD19200);
    ui->baudRateBox->addItem("38400", BAUD38400);
    ui->baudRateBox->addItem("57600", BAUD57600);

    ui->parityBox->addItem("NONE", PAR_NONE);
    ui->parityBox->addItem("ODD", PAR_ODD);
    ui->parityBox->addItem("EVEN", PAR_EVEN);

    ui->dataBitsBox->addItem("5", DATA_5);
    ui->dataBitsBox->addItem("6", DATA_6);
    ui->dataBitsBox->addItem("7", DATA_7);
    ui->dataBitsBox->addItem("8", DATA_8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->stopBitsBox->addItem("1", STOP_1);
    ui->stopBitsBox->addItem("2", STOP_2);

    ui->jsScanButton->setEnabled(true);
    ui->jsStartButton->setEnabled(true);
    ui->jsStopButton->setEnabled(false);
    ui->jsScanButton->setFocus();

    PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
    port = new QextSerialPort(ui->portBox->currentText(), settings, QextSerialPort::EventDriven);

    connect(ui->jsScanButton, SIGNAL(clicked()), this, SLOT(onjsScanButtonClicked()));
    connect(ui->jsStartButton, SIGNAL(clicked()), this, SLOT(onjsStartButtonClicked()));
    connect(ui->jsStopButton, SIGNAL(clicked()), this, SLOT(onjsStopButtonClicked()));

    connect(sendTimer, SIGNAL(timeout()), this, SLOT(onSendData()));

    connect(ui->baudRateBox, SIGNAL(currentIndexChanged(int)), SLOT(onBaudRateChanged(int)));
    connect(ui->parityBox, SIGNAL(currentIndexChanged(int)), SLOT(onParityChanged(int)));
    connect(ui->dataBitsBox, SIGNAL(currentIndexChanged(int)), SLOT(onDataBitsChanged(int)));
    connect(ui->stopBitsBox, SIGNAL(currentIndexChanged(int)), SLOT(onStopBitsChanged(int)));
    connect(ui->portBox, SIGNAL(editTextChanged(QString)), SLOT(onPortNameChanged(QString)));
    connect(ui->spStartButton, SIGNAL(clicked()), this, SLOT(onspStartButtonClicked()));
    connect(ui->spStopButton, SIGNAL(clicked()), this, SLOT(onspStopButtonClicked()));
    connect(port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(onClearBtnClicked()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(onSaveBtnClicked()));
}

JSWidget::~JSWidget()
{
    delete ui;
}

void JSWidget::onSendData()
{
//    if(port->isOpen())
    {
    //............................................
        QByteArray data;
        data.append(QString("%1").arg(0xAAAA, 2, 16, QChar('0')).toLatin1());
        data.append(QString("%1").arg(AddEvenParity(mCmd->gDirection1 << 4, 16)&0xFFFF, 4, 16, QChar('0')).toLatin1());
        data.append(QString("%1").arg(AddEvenParity(mCmd->gDirection2 << 4, 16)&0xFFFF, 4, 16, QChar('0')).toLatin1());
        data.append(QString("%1").arg(AddEvenParity(mCmd->gEngineLevel << 4, 16)&0xFFFF, 4, 16, QChar('0')).toLatin1());
        data.append(QString("%1").arg(AddEvenParity(mCmd->gDirection3 << 4, 16)&0xFFFF, 4, 16, QChar('0')).toLatin1());
        data.append(QString("%1").arg(AddEvenParity(mCmd->gControlMode*128 + mCmd->gFunctionBits*2, 8)&0xFF, 2, 16, QChar('0')).toLatin1());
        data.append(QString("%1").arg(0xFFFF, 2, 16, QChar('0')).toLatin1());

        QByteArray text = QByteArray::fromHex(data);
        port->write(text.data());

        data.append("\n");
        ui->sendText->moveCursor(QTextCursor::End);
        ui->sendText->insertPlainText(QString::fromLatin1(data));
    //.............................................
    }
//    else
//    {
//        qDebug() << "Warning: the serial port can not be opened !";
//    }
}

void JSWidget::onJoysChanged(int count, QListIterator<Joystick *> i)
{
    if(count > 0)
    {
        ui->jsStartButton->setEnabled(true);
        ui->jsTimeOutSpinBox->setEnabled(true);

        Joystick *j;
        QString txt;
        while (i.hasNext())
        {
            j = i.next();
            txt = QString("#%1").arg(j->name);
            ui->jsListBox->addItem(txt);
        }
    }
    else
    {
        ui->jsListBox->clear();
        ui->jsStartButton->setEnabled(false);
        ui->jsTimeOutSpinBox->setEnabled(false);
        qDebug() << "no joystick connected!";
    }
}

void JSWidget::onDataChanged(int count, QListIterator<Joystick *> i)
{
    if (count == 0)
    {
        return;
    }

    Joystick *j;
    if(!i.hasNext())
    {
        return;
    }
    j = i.next();

    //............................................................................
    mCmd->gEngineLevel = (int)(j->axes.value(2)*1000/65536+1500);
    mCmd->gDirection1 = (int)(j->axes.value(0)*1000/65536+1500);
    mCmd->gDirection2 = (int)(j->axes.value(1)*1000/65536+1500);
    mCmd->gDirection3 = (int)(j->axes.value(3)*1000/65536+1500);
    mCmd->gControlMode = (int)(1*(int)(ui->yaokongqiFuncBit->isChecked()));//0:shangweiji, 1:yaokongqi
    mCmd->gFunctionBits = (int)(1*(int)(ui->ziwenFuncBit->isChecked())+2*(int)(ui->dinggaoFuncBit->isChecked())+3*(int)(ui->fanhangFuncBit->isChecked()));
    //............................................................................

    ui->rollBar->setValue(mCmd->gDirection1);
    ui->pitchBar->setValue(mCmd->gDirection2);
    ui->throttleBar->setValue(mCmd->gEngineLevel);
    ui->yawBar->setValue(mCmd->gDirection3);
}

void JSWidget::onjsScanButtonClicked()
{
    ui->jsListBox->clear();

    emit scan();
}

void JSWidget::onjsStartButtonClicked()
{
    ui->jsStartButton->setEnabled(false);
    ui->jsStopButton->setEnabled(true);
    ui->jsScanButton->setEnabled(false);
    ui->jsListBox->setEnabled(false);
    ui->jsTimeOutSpinBox->setEnabled(false);

    emit start(ui->jsTimeOutSpinBox->value());
}

void JSWidget::onjsStopButtonClicked()
{
    ui->jsStartButton->setEnabled(true);
    ui->jsStopButton->setEnabled(false);
    ui->jsScanButton->setEnabled(true);
    ui->jsListBox->setEnabled(true);
    ui->jsTimeOutSpinBox->setEnabled(true);

    emit stop();
}

void JSWidget::onPortNameChanged(const QString &)
{
    if (port->isOpen())
    {
        port->close();
    }
}

void JSWidget::onBaudRateChanged(int idx)
{
    port->setBaudRate((BaudRateType)ui->baudRateBox->itemData(idx).toInt());
}

void JSWidget::onParityChanged(int idx)
{
    port->setParity((ParityType)ui->parityBox->itemData(idx).toInt());
}

void JSWidget::onDataBitsChanged(int idx)
{
    port->setDataBits((DataBitsType)ui->dataBitsBox->itemData(idx).toInt());
}

void JSWidget::onStopBitsChanged(int idx)
{
    port->setStopBits((StopBitsType)ui->stopBitsBox->itemData(idx).toInt());
}

void JSWidget::onspStartButtonClicked()
{
    if (!port->isOpen())
    {
        port->setPortName(ui->portBox->currentText());
        port->open(QIODevice::ReadWrite);
    }

    if(port->isOpen())
    {
        sendTimer->setInterval(ui->sendTimespinBox->value());
        sendTimer->start();
        ui->sendTimespinBox->setEnabled(false);
        ui->portBox->setEnabled(false);
        ui->baudRateBox->setEnabled(false);
        ui->parityBox->setEnabled(false);
        ui->dataBitsBox->setEnabled(false);
        ui->stopBitsBox->setEnabled(false);
        ui->spStartButton->setEnabled(false);
        ui->spStopButton->setEnabled(true);
    }
    else
    {
        qDebug() << "Cannot open this port !";
    }
}

void JSWidget::onspStopButtonClicked()
{
    port->close();

    if(!port->isOpen())
    {
        sendTimer->stop();
        ui->sendTimespinBox->setEnabled(false);
        ui->portBox->setEnabled(true);
        ui->baudRateBox->setEnabled(true);
        ui->parityBox->setEnabled(true);
        ui->dataBitsBox->setEnabled(true);
        ui->stopBitsBox->setEnabled(true);
        ui->spStartButton->setEnabled(true);
        ui->spStopButton->setEnabled(false);
    }
    else
    {
        qDebug() << "Cannot close this port !";
    }
}

void JSWidget::onReadyRead()
{
    if (port->bytesAvailable())
    {
        ui->recvText->moveCursor(QTextCursor::End);

        if(!ui->hexBox->isChecked())
        {
            ui->recvText->insertPlainText(QString::fromLatin1(port->readAll()));
        }
        else
        {
            ui->recvText->insertPlainText(QString::fromLatin1(port->readAll().toHex()));
        }
    }
}

void JSWidget::onClearBtnClicked()
{
    ui->recvText->clear();
}

void JSWidget::onSaveBtnClicked()
{
    QDateTime date_time;
    QString fileName = date_time.currentDateTime().toString("yyyy-MM-dd-HH-mm-ss") + ".txt";
    QFile file(fileName);

    if(file.exists())
    {
        qDebug() << "File already exists!";
        return;
    }

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Cannot write file!";
        return;
    }

    QTextStream fileIn(&file);
    fileIn << ui->recvText->toPlainText();
    file.flush();
    file.close();
    QMessageBox::about(this, "OK", "Data saved.");
}

int JSWidget::AddEvenParity(int value, int bits)
{
    if(16 == bits)
    {
        int x = value & 0xFFFF;
        x ^= x >> 8;
        x ^= x >> 4;
        x ^= x >> 2;
        x ^= x >> 1;
        x &= 1;
        return ( x + value);
    }
    else if(8 == bits)
    {
        int x = value & 0xFF;
        x ^= x >> 4;
        x ^= x >> 2;
        x ^= x >> 1;
        x &= 1;
        return ( x + value);
    }
    else
    {
        return 0x01; //error
    }
}
