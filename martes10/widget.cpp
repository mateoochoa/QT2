#include "widget.h"
#include "ui_widget.h"
#include<QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        /*qDebug()<<"Name:"<<info.portName();
        qDebug()<<"Manufacturer:"<<info.manufacturer();
        qDebug()<<"ID:"<<info.vendorIdentifier();
        qDebug()<<"Vendor:"<<info.hasVendorIdentifier();
        qDebug()<<"Product:"<<info.hasProductIdentifier();
        qDebug()<<"VendorID:"<<info.vendorIdentifier();
        qDebug()<<"ProductID:"<<info.productIdentifier();*/
        if(info.vendorIdentifier()==vendor && info.productIdentifier()==product){
            portn=info.portName();
            ui->pushButton->setEnabled(true);
            ui->label->setText("Conectado");
            break;
        }else{
            ui->pushButton->setEnabled(false);
        }
    }
    sl=new QSerialPort(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::OpenSerialPort(QString p)
{
    if(sl->isOpen()){
        sl->close();
    }
    disconnect(sl,SIGNAL(readyRead()),this,SLOT(readSerial()));
    sl->setPortName(p);
    sl->setBaudRate(QSerialPort::Baud115200);
    sl->setDataBits(QSerialPort::Data8);
    sl->setParity(QSerialPort::NoParity);
    sl->setStopBits(QSerialPort::OneStop);
    sl->setFlowControl(QSerialPort::NoFlowControl);
    connect(sl,SIGNAL(readyRead()),this,SLOT(readSerial()));
    if(sl->open(QIODevice::ReadWrite)){
        ui->pushButton->setText("Cerrar");
    }else{
        QMessageBox::critical(this,tr("Error"),sl->errorString());
    }
}


void Widget::on_pushButton_clicked()
{
    if(sl->isOpen()){
        sl->close();
        ui->pushButton->setText("Abrir");
    }else{
        qDebug()<<"Ne";
        OpenSerialPort(portn);
    }
}

void Widget::readSerial()
{
    QByteArray serialData=sl->readAll();
    QString data=QString::fromStdString(serialData.toStdString());
    ui->datoss->appendPlainText(data);
}
