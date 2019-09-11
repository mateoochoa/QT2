#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QTimer>
#include <QString>
#include <stdio.h>

QTimer *timer1 = new QTimer();
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
            qDebug()<<"true";
            ui->conect->setText("   Conectado");
            break;
        }else{
            ui->pushButton->setEnabled(false);
        }
    }
    {
        ui->setupUi(this);
        connect(timer1,                            //conexion entre el timer y la funcion incrementar
                SIGNAL(timeout()),
                this,
                SLOT(Incrementar())
                );
    }
    sl=new QSerialPort(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::Incrementar(){ //con esta funcion incrementamos el valor de una variable cada vez que el timer me dice
char a[1] ={0x01};
sl->write(a,1);

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
        if(timer1->isActive()){
            timer1->stop();
        }
        ui->pushButton->setText("Abrir");
    }else{
        OpenSerialPort(portn);
        timer1->start(1000);//el timer cada segundo ejecuta la funcion incrementar
    }
}

void Widget::readSerial()
{
    QByteArray serialData=sl->readAll();
    QString data=QString::fromStdString(serialData.toStdString());
    qDebug()<<"data :"<<data;
    suf(data);
}
void Widget :: suf(QString t1){
double t2=0;
double t=t1.toDouble();
QString sufig,text;


if (t/1e3>=1)
{    sufig="Khz";
    t2=(t/1e3);
    }else if (t>=1){
    sufig="hz";
    }
    ui->Progres->setValue((int)t2);
    ui->label->setNum((int)t2);
    ui->sufF1->setText(sufig);
    ui->sufPS1->setText(sufig);
}
