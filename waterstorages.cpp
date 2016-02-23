#include "waterstorages.h"
#include "ui_waterstorages.h"
#include<QDebug>
#define SETITEM(r, c, val) {item = ui->tblData->item(r, c);if (item) item->setText(val); else {item = new QTableWidgetItem;item->setText(val);ui->tblData->setItem(r, c, item);}}

WaterStorages::WaterStorages(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaterStorages)
{
    ui->setupUi(this);
    port = NULL;
    state = wstNone;
    mtx = new QMutex;
    ui->tblData->setRowCount(0);
    int cols = 8;
    this->ui->tblData->setColumnCount(cols);
    this->ui->tblData->clear();
    int cc = 0;
    this->ui->tblData->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Индекс")));
    this->ui->tblData->setColumnWidth(cc++, 60);
    this->ui->tblData->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Название")));
    this->ui->tblData->setColumnWidth(cc++, 300);
    this->ui->tblData->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("ёмкость, мл")));
    this->ui->tblData->setColumnWidth(cc++, 80);
    this->ui->tblData->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("пин насоса")));
    this->ui->tblData->setColumnWidth(cc++, 80);
    this->ui->tblData->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("использовать")));
    this->ui->tblData->setColumnWidth(cc++, 80);
    this->ui->tblData->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Приоритет")));
    this->ui->tblData->setColumnWidth(cc++, 80);
    this->ui->tblData->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("флаги")));
    this->ui->tblData->setColumnWidth(cc++, 80);
    this->ui->tblData->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Использовано, мл")));
    this->ui->tblData->setColumnWidth(cc++, 80);
}

WaterStorages::~WaterStorages()
{
    QObject::disconnect(port,  SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    delete mtx;
    mtx = NULL;
    delete ui;
}

void WaterStorages::setPort(QextSerialPort *_port)
{
    this->port = _port;
    QObject::connect(port,  SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
}

void WaterStorages::on_btnReadData_clicked()
{
    if ( !(port && port->isOpen())) {
        qDebug()<<"no port opened";
        return;
    }
    state = wstReadData;
    QMutexLocker ml(mtx);
    buf.clear();
    port->write("wst get all;\r\n");
}

void WaterStorages::onDataAvailable()
{
    QMutexLocker ml(mtx);
    if (!port->isOpen()) {
        qDebug()<<"port closed!";
    }
    buf.append(port->readAll());
    qDebug()<<buf;
    if (!buf.contains(";")) return;
    if (state == wstReadData) {
        QRegExp cleaner("\\r|\\n||\\t|\\v|\\s{2,}");
        qDebug()<<"buffer:"<<buf;
        buf = buf.replace(cleaner, "");
        qDebug()<<"cleaned buffer:"<<buf;
        QRegExp parcer("^(\\d+)\\,([^\\,]+)\\,(\\d+)\\,(\\d+)\\,(\\d+)\\,(\\d+)\\,(\\d+)\\,(\\d+)\\;$");
        if (parcer.indexIn(buf) > -1) {
            int index = parcer.cap(1).toInt();
            if (index >= ui->tblData->rowCount()) {
                ui->tblData->setRowCount(index + 1);
            }
                QTableWidgetItem* item = NULL;
                SETITEM(index, 0, parcer.cap(1));
                SETITEM(index, 1, parcer.cap(2));
                SETITEM(index, 2, parcer.cap(3));
                SETITEM(index, 3, parcer.cap(4));
                SETITEM(index, 4, parcer.cap(5));
                SETITEM(index, 5, parcer.cap(6));
                SETITEM(index, 6, parcer.cap(7));
                SETITEM(index, 7, parcer.cap(8));
        }
        buf.clear();
        //state = wstNone;
    }
}

void WaterStorages::on_btnMarkRefilled_clicked()
{
    if ( !(port && port->isOpen())) return;
    state = wstNone;
    QMutexLocker ml(mtx);
    buf.clear();
    QString cmd("wst clr %1;\r\n");
    cmd = cmd.arg(ui->tblData->itemAt(ui->tblData->currentRow(), 0)->text());
    qDebug()<<cmd;
    port->write(cmd.toLocal8Bit());
}

void WaterStorages::on_btnSendData_clicked()
{
    QTextStream stm;
    QString cmd;
    QMutexLocker ml(mtx);
    buf.clear();
    state = wstNone;
    if (port && port->isOpen()) {
        qDebug()<<"port not opened";
        return;
    }
    stm.setString(&cmd);
    for (int i = 0; i < 4; ++i) {
        cmd.clear();
        stm<<"wst be "<<i<<",0;";
        port->write(cmd.toLatin1());
    }

    for (int i = 0; i < ui->tblData->rowCount(); ++i) {
        cmd.clear();
        stm<<"wst set "<<i<<",";
        for(int f = 1; f <=4; ++f) {
            qDebug()<<ui->tblData->item(i, f)->text()<<" "<<(ui->tblData->item(i, f)!=NULL?ui->tblData->item(i, f)->text():"0");
            stm<<(ui->tblData->item(i, f)!=NULL?ui->tblData->item(i, f)->text():"0");
            stm<<",";
        }
        stm<<1;
        stm<<","<<(ui->tblData->item(i, 5)!=NULL ? ui->tblData->item(i, 5)->text():"0")<<","<<(ui->tblData->item(i, 6)!=NULL ? ui->tblData->item(i, 5)->text():"0")<<";";
        stm<<"wst be "<<i<<",1;\r\n";
        qDebug()<<cmd;
        if (port && port->isOpen()) {
            port->write(cmd.toLocal8Bit());
        }
    }
}

void WaterStorages::on_btnAddStorage_clicked()
{
    ui->tblData->setRowCount(ui->tblData->rowCount() + 1);
}

void WaterStorages::on_btnDeleteStorage_clicked()
{
    if (ui->tblData->currentRow() >=0 ) {
        ui->tblData->removeRow(ui->tblData->currentRow());
    }
}
