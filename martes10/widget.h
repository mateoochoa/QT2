#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QtSerialPort>
#include<QSerialPortInfo>
#include<QMessageBox>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();
    void readSerial();
    void Incrementar();

private:
    Ui::Widget *ui;
    QSerialPort *sl=nullptr;
    uint32_t vendor=1155,product=22336;
    QString portn;
    void suf(QString t1);
    void OpenSerialPort(QString p);
};

#endif // WIDGET_H
