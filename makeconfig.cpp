#include "makeconfig.h"
#include "ui_makeconfig.h"
#include "data_structs.h"
#include <QSettings>
#include <QFileDialog>
#include <QProcess>
#include "globalstatedialog.h"
#include "waterstorages.h"
#include "visualconfig.h"

#include<QDebug>
#include <algorithm>
#include <vector>
#include<QMutexLocker>

#define SETITEM(r, c, val) {item = ui->tblConfig->item(r, c);if (item) item->setText(val); else {item = new QTableWidgetItem;item->setText(val);ui->tblConfig->setItem(r, c, item);}}
#define INT2STR(val) (QString("%1").arg(val))

MakeConfig::MakeConfig(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MakeConfig)
{
    mtx = new QMutex();
    ui->setupUi(this);
    ui->cbChipType->clear();
    ui->cbChipType->addItem("CD4067", 16);
    ui->cbChipType->addItem("HCF4051", 8);
    ui->cbChipType->setCurrentIndex(0);
    is_new = true;
    port = 0;
    this->modified = false;
    buf.resize(0);
    last_found = -1;
    this->state =stNone;
    this->pots = 0;
    this->scanner = NULL;
    this->ui->cmbDevice->setEnabled(false);
    this->ui->btnConnect->setEnabled(false);
    this->ui->btDetect->setEnabled(false);
    this->ui->btDetect2->setEnabled(false);
    this->ui->btnGetVals->setEnabled(false);
    this->ui->btSend->setEnabled(false);
    this->ui->btApplyDetected->setEnabled(false);
//   this->ui->btSave->setEnabled(false);
    QObject::connect(this->ui->mnuExit, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect(this->ui->mnuSetup, SIGNAL(triggered()),this, SLOT(onSetup()));
    QObject::connect(this->ui->mnuLoadFromDev, SIGNAL(triggered()), this, SLOT(mnu_onLoadFromDev()));
    QObject::connect(this->ui->mnuAddRow, SIGNAL(triggered()), this, SLOT(on_mnuAddRow()));
    QObject::connect(this->ui->mnuTestWatering, SIGNAL(triggered()), this, SLOT(on_mnuTestWatering()));
    QObject::connect(ui->actWST, SIGNAL(triggered()), this, SLOT(on_actWST_clicked()));
    QObject::connect(this->ui->mnuVisualCfg, SIGNAL(triggered()), this, SLOT(showVisualCfg()));
    ui->statusBar->showMessage(QString::fromLocal8Bit("Для начала работы найдите устройства"));
}

MakeConfig::~MakeConfig()
{
    if (this->port) {
        this->port->close();
        this->port = NULL;
    }
    delete ui;
}
/*
void MakeConfig::setPort(QextSerialPort *_port)
{
    this->port = _port;
    if (this->port && port->isOpen()) {
        QObject::connect(this->port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
        qDebug()<<"SLOT readyRead connected";
    }
}*/

void MakeConfig::on_spnChips_valueChanged(int arg1)
{
    this->resizeTable();
}

void MakeConfig::resizeTable()
{

    int rows = this->ui->spnRows->value();

    if (modified && (rows < this->ui->tblConfig->rowCount())) {
        ui->statusBar->showMessage(QString::fromLocal8Bit("Нельзя уменьшить кол-во строк в несохранённой конфигурации"));
        return;
    }
    ui->tblConfig->setRowCount(rows);
    int cols = 11+3;
    this->ui->tblConfig->setColumnCount(cols);
    this->ui->tblConfig->clear();
    int cc = 0;
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("флаги")));
    this->ui->tblConfig->setColumnWidth(cc++, 20);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Растение")));
    this->ui->tblConfig->setColumnWidth(cc++, 300);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("адрес pcf")));
    this->ui->tblConfig->setColumnWidth(cc++, 40);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Пин pcf")));
    this->ui->tblConfig->setColumnWidth(cc++, 40);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("X")));
    this->ui->tblConfig->setColumnWidth(cc++, 40);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Y")));
    this->ui->tblConfig->setColumnWidth(cc++, 70);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Доза")));
    this->ui->tblConfig->setColumnWidth(cc++, 80);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Программа")));
    this->ui->tblConfig->setColumnWidth(cc++, 75);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Мин")));
    this->ui->tblConfig->setColumnWidth(cc++, 75);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Макс")));
    this->ui->tblConfig->setColumnWidth(cc++, 75);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Тек.пок.")));
    this->ui->tblConfig->setColumnWidth(cc++, 80);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Вкл/Выкл")));
    this->ui->tblConfig->setColumnWidth(cc++, 80);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Полив/подсушка")));
    this->ui->tblConfig->setColumnWidth(cc++, 80);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Уже вылито")));
    this->ui->tblConfig->setColumnWidth(cc++, 80);
}

void MakeConfig::on_cbChipType_currentIndexChanged(int index)
{
    this->resizeTable();
}

void MakeConfig::on_btDetect_clicked()
{
    if (!port) {
        ui->statusBar->showMessage(QString::fromLocal8Bit("Нет соединения"));
        return;
    }
    last_found = -1;
    this->state = stDetectPinStep1;
    buf.clear();
    port->write("getall\r\n");
    ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск сенсора, шаг 1..."));
}

void MakeConfig::on_btDetect2_clicked()
{
    if (!port) {
        ui->statusBar->showMessage(QString::fromLocal8Bit("Не подключён"));
        return;
    }
    last_found = -1;
    buf.clear();
    port->flush();
    this->state = stDetectPinStep2;
    port->write("getall\r\n");
    //port->flush();
    ui->statusBar->showMessage(QString::fromLocal8Bit("Шаг 2:ждём ответ"));
}

void MakeConfig::on_tblConfig_itemChanged(QTableWidgetItem *item)
{
    this->modified = true;
}

void MakeConfig::onDataAvailable()
{
    QMutexLocker mtxlock(mtx);
    QByteArray data =  port->readAll();
    if(data[0]==13 || data[0]==10) return;

    buf.append(data);
qDebug()<<"state"<<state<<"buf="<<buf;
    if (state == stNone) return;
    if (state == stReadCurState) {
        ui->statusBar->showMessage(QString::fromLocal8Bit("Получение текущего состояния..."));
        if (!buf.contains(";")) return;
        ui->statusBar->showMessage(QString::fromLocal8Bit("Дешифровка текущего состояния..."));
        QString s = QString(buf);
        QStringList devs = s.split(QString(","));
        if (devs.count() < 2) {
            state = stNone;
            qDebug()<<"no sensor devices found";
            buf.clear();
            return;
        }
        QString cmd;
        for (int i=0; i< devs.count()-1; ++i ) {
            cmd.append(QString("iic %1,*;").arg(devs[i]));
        }
        qDebug()<<"iic cmd: "<<cmd;
        buf.clear();
        port->write(cmd.toLocal8Bit());
        state = stReadCurState2;
        return;
    } else if (state == stReadCurState2) {
        if (!buf.contains(";")) {
            return;
        }
        qDebug()<<"got iic data:"<<buf;
        QString data(buf.trimmed());
        data.chop(1);
        QStringList parts = data.split(",");
        if (parts.length() < 3) {
            qDebug()<<"bad data:"<<data;
            buf.clear();
            return;
        }
        int r=0;
        while (r < this->ui->tblConfig->rowCount()) {
            if (! (ui->tblConfig->item(r, 2) && ui->tblConfig->item(r, 3))) {
                ++r;
                continue;
            }
            if ( (ui->tblConfig->item(r, 3)->text() == parts[1]) && (ui->tblConfig->item(r, 2)->text() == parts[0]) ) {

                QTableWidgetItem *item= new QTableWidgetItem(parts[2]);
                ui->tblConfig->setItem(r, 10, item);
                break;
            }
            ++r;
        }//while
        buf = buf.remove(0, buf.indexOf(";") + 1);
    } else if (state == stSendConfigLines) {
        //qDebug()<<buf.contains("OK");
        if (buf.contains("OK")) {
            buf.clear();
            ui->statusBar->showMessage(QString::fromLocal8Bit("Отправляем строку конфигурации №%1").arg(lsi+1));
            this->sendConfigLine(++lsi);
        } else if (buf.contains("setup()")) {
            buf.clear();
            this->sendConfigLine(lsi);
        }
        return;
    }else if (stSendSensorsCount == state) {
        //qDebug()<<buf.contains("SET");
        if (buf.contains(("SET"))) {
            state = stSendPlantNames;
            this->lsi  = 0;
            ui->statusBar->showMessage(QString::fromLocal8Bit("Начинаем отправку названий растений..."));
            this->port->write("\7");
            this->port->flush();
            sleep(1);
            this->sendPlantName(lsi);
        }
        return;
    } else if (state == stSendPlantNames) {
        //qDebug()<<buf.contains("OK");
        if (buf.contains("OK")) {
            buf.clear();
            ui->statusBar->showMessage(QString::fromLocal8Bit("Отправка названия из строки %1").arg(lsi+1));
            this->sendPlantName(++lsi);
        }
        return;
    } else if (state == stGetW2Dsizes) {
        if (buf.contains(";")) {
            QRegExp re("^.*(\\d+)\\s*\\,\\s*(\\d+).*$");
            if (re.indexIn(buf) > -1) {
                qDebug()<<re.cap(1)<< re.cap(1).toInt()<<re.cap(2)<< re.cap(2).toInt();
                ui->spinX->setMaximum(re.cap(1).toInt());
                ui->spinY->setMaximum(re.cap(2).toInt());
            } else {
                qDebug()<<"not matched";
            }
            state = stNone;
        }
    } else if (state == stReadConfig) {
        if (buf.contains(";")) {
            if (last_index == -1) {
                qDebug()<<"got pot cnt";
                buf = buf.trimmed();
                buf.chop(1);
                pots_total = buf.toInt();
                last_index = 0;
                buf.clear();
                this->port->write(QString("pot get %1;\r\n").arg(last_index).toLocal8Bit());
                this->ui->tblConfig->setRowCount(0);
                this->ui->tblConfig->setRowCount(pots_total);
            } else if (buf.length() > 10 && buf.contains(",")) {
                buf = buf.trimmed();
                buf.chop(1);
                QStringList parts = buf.split(",");
                if (parts.length() >=14) {
                    QTableWidgetItem*item;
                    SETITEM(last_index, 0, parts[1]);//флаги
                    SETITEM(last_index, 1, parts[2]);//растение
                    SETITEM(last_index, 2, parts[3]);//чип
                    SETITEM(last_index, 3, parts[4]);//пин
                    SETITEM(last_index, 4, parts[5]);// Х
                    SETITEM(last_index, 5, parts[6]);// У
                    SETITEM(last_index, 6, parts[7]);// доза
                    SETITEM(last_index, 7, parts[8]);// программа
                    SETITEM(last_index, 8, parts[9]);// мин
                    SETITEM(last_index, 9, parts[10]);// макс
                    SETITEM(last_index, 10, QString("?"));// тек.знач-ие
                    SETITEM(last_index, 11, parts[13]);// вкл/выкл
                    SETITEM(last_index, 12, (parts[12]=="1"?QString::fromLocal8Bit("сушим"):QString::fromLocal8Bit("льём")));// состояние(полив/подсушка)
                    SETITEM(last_index, 13, parts[14]);// вылито
                    ++last_index;
                    PlantData pd;
                    pd.index = parts[0].toInt();
                    pd.flags = parts[1].toInt();
                    pd.name = parts[2];
                    pd.chip = parts[3].toInt();
                    pd.pin = parts[4].toInt();
                    pd.X = parts[5].toInt();
                    pd.Y = parts[6].toInt();
                    pd.portion = parts[7].toInt();
                    pd.pgm = parts[8].toInt();
                    pd.min = parts[9].toInt();
                    pd.max = parts[10].toInt();
                    pd.en = parts[13].toInt();
                    this->pots_data.push_back(pd);
                } else {
                    qDebug()<<"re-query pot "<<last_index;
                }
                buf.clear();
                if (last_index < pots_total) {
                    qDebug()<<"query pot "<<last_index;
                    this->port->write(QString("pot get %1;\r\n").arg(last_index).toLocal8Bit());
                    //this->port->flush();
                } else {
                    state = stGetW2Dsizes;
                    this->port->write("WSZ;\r\n");
                }
            } else {
                this->port->write(QString("pot get %1;\r\n").arg(last_index).toLocal8Bit());
                buf.clear();
            }
            return;
        } else {
            return;
        }
        if (!buf.contains(";;")) return;
        buf = buf.trimmed();
        qDebug()<<"READ CONFIG"<<"\n----\n"<<buf<<"\n-----";
        QRegExp re("\\{(\\d+)\\};");
        re.indexIn(buf);
        int n_rows = re.cap(1).toInt();
        this->ui->tblConfig->setRowCount(0);
        this->ui->tblConfig->setRowCount(n_rows);

        buf= buf.replace(re, "").trimmed();
        QStringList list = buf.split(";");
        qDebug()<<list;
        QRegExp il("^(.*)\\[(.*)\\]$");
        int r = 0;
        for (int i = 0; i < list.length(); ++i) {
            //qDebug()<<list[i];
            if (il.indexIn(list[i].trimmed()) > -1) {
                QString nums = il.cap(1), name = il.cap(2);
                nums = nums.replace(QRegExp("\\s+"), "");
                //qDebug()<<"nums:"<<nums;
                QStringList numz = nums.split(",");
                QTableWidgetItem*item;
                SETITEM(r, 0, name);
                SETITEM(r, 1, numz[0]);
                SETITEM(r, 2, numz[1]);
                SETITEM(r, 3, numz[2]);
                SETITEM(r, 4, numz[3]);
                SETITEM(r, 5, numz[4]);
                SETITEM(r, 6, numz[5]);
                SETITEM(r, 7, numz[6]);
                SETITEM(r, 8, numz[7]);
                ++r;
             //   qDebug()<<numz;
            }
        }
        buf.clear();
        state = stNone;
        return;
    }
    //QStringList lines = buf.split("\n");
    //first line -- cmd echo
    //qDebug()<<data<<data.contains(';');
    if (buf.contains(';')) {
        buf = buf.trimmed();
        //qDebug()<<buf;
        if (state == stDetectPinStep1) {
            //test1.resize(0);
        } else if(state == stDetectPinStep2) {
            //test2.resize(0);
        } else {
            ui->statusBar->showMessage(QString::fromLocal8Bit("Неправильный код состояния. Возможно, идёт отсылка/приём данных..."));
            data.clear();
            return;
        }
            QString str = QString(buf);
            QTextStream st(&str,  QIODevice::ReadOnly);
            int n;
            st>>n;
            //qDebug() << "n="<<n;
            for (int i = 0; i < n; ++i) {
                int val;
                st>>val;
                if (state == stDetectPinStep1) {
                    //test1.push_back(val);
                } else if (state ==stDetectPinStep2) {
                    //test2.push_back(val);
                }
            }
            if (state == stDetectPinStep1) {
                ui->statusBar->showMessage(QString::fromLocal8Bit("Готов к шагу 2"));
            } else if (state == stDetectPinStep2) {
                qDebug()<<"state"<<state<<"buf="<<buf;
              /*  if (test1.size()!=test2.size()) {
                    ui->statusBar->showMessage(QString::fromLocal8Bit("Ошибка: не совпадает размер данных"));
                    buf.clear();
                    //qDebug()<<"state=0, ln 216";
                    state = stNone;
                    return;
                }
                int max_df = 0, max_index=-1, _cdf;
                for (int i=0;i<test1.size(); ++i) {
                    _cdf = abs(test1.at(i)-test2.at(i));
                    if (_cdf > max_df) {
                        qDebug()<<"delta: "<<_cdf;
                        max_index = i;
                        max_df = _cdf;
                    }
                }
                if (max_index >=0 ) {
                    qDebug()<<"max_index "<<max_index;
                    int pins = ui->cbChipType->itemData((ui->cbChipType->currentIndex())).toInt();
                    qDebug()<<"max_index "<<max_index;
                    ui->statusBar->showMessage(QString::fromLocal8Bit("Чип %1 пин %2").arg(max_index/pins).arg(max_index%pins));
                    this->last_found = max_index;
                }
                state = stNone;
                //qDebug()<<"state=0, ln234";
                */
            }
    }
}

void MakeConfig::on_btApplyDetected_clicked()
{
    //qDebug()<<ui->tblConfig->currentRow();
    QTableWidgetItem*item = ui->tblConfig->item(ui->tblConfig->currentRow(), 1);
    //qDebug()<<item;
    int pins = ui->cmbDevice->itemData((ui->cbChipType->currentIndex())).toInt();
    if (item) {
        item->setText(QString("%1").arg(last_found/pins));
    } else {
        item = new QTableWidgetItem;
        item->setText(QString("%1").arg(last_found/pins));
        ui->tblConfig->setItem(ui->tblConfig->currentRow(), 1, item);
    }

    item = ui->tblConfig->item(ui->tblConfig->currentRow(), 2);
    if (item) {
        item->setText(QString("%1").arg(last_found%pins));
    } else {
        item = new QTableWidgetItem;
        item->setText(QString("%1").arg(last_found%pins));
        ui->tblConfig->setItem(ui->tblConfig->currentRow(), 2, item);
    }
}

void MakeConfig::on_btSend_clicked()
{
    //qDebug()<<"state:"<<state;
    if (state == stSendConfigLines) {
        //this->port->write("\7");
        //this->port->flush();
        this->sendConfigLine(lsi);
        return;
        //this->state = stNone;
         //ui->statusBar->showMessage(QString::fromLocal8Bit("Неверный статус. Система занята."));
         //return;
    } else if (state == stSendPlantNames) {
        this->sendPlantName(lsi);
        return;
    }
    ui->statusBar->showMessage(QString::fromLocal8Bit("Начинаем отправлять конфигурацию..."));
    state = stSendConfigLines;
    lsi = 0;
    this->sendConfigLine(lsi);
}

void MakeConfig::sendConfigLine(int index)
{
    if (!this->port || !this->port->isOpen()) {
        ui->statusBar->showMessage(QString::fromLocal8Bit("Порт недоступен. Обрыв связи?"));
        return;
    }

    if ( (index >= this->ui->tblConfig->rowCount()) || !this->ui->tblConfig->item(index, 1 )
         || this->ui->tblConfig->item(index, 1 )->text().length() < 1) {
        state = stSendSensorsCount;
      //  qDebug()<<"SET SENSORS "<<index;
        this->port->write(QString("pot set count %1\n").arg(index).toLocal8Bit());
        return;
    }
    //qDebug()<<"LSI"<< index;
   /* if (!this->ui->tblConfig->item(index, 1 )) {
        qDebug()<<"end of send";
        return;
    }*/
/*
    qDebug()<<this->ui->tblConfig->item(index, 1)->text()<<//chip
            this->ui->tblConfig->item(index, 2)->text()<<//pin
            this->ui->tblConfig->item(index, 3)->text()<<//doser
            this->ui->tblConfig->item(index, 4)->text()<< // cup
            this->ui->tblConfig->item(index, 5)->text()<<//portion
            this->ui->tblConfig->item(index, 6)->text()<<//min sensor val
            this->ui->tblConfig->item(index, 7)->text()<<//max sensor val
            this->ui->tblConfig->item(index, 8)->text();//day max portion
    */
    int test = this->ui->tblConfig->item(index, 2)->text().toInt()+//chip
            this->ui->tblConfig->item(index, 3)->text().toInt()+//pin
            this->ui->tblConfig->item(index, 8)->text().toInt()+//doser
            this->ui->tblConfig->item(index, 9)->text().toInt()+ // cup
            this->ui->tblConfig->item(index, 4)->text().toInt()+//portion
            this->ui->tblConfig->item(index, 5)->text().toInt();
    if (test) {
        //pot set <index>,<flags>,<dev>,<pin>,<dry>,<wet>,<x>,<y>,<name>,<pgm>,<airTime>,<state>,<enabled>,<ml>,<watered>;
        QString line("pot set %1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15;\r\n");

        line=line.arg(
                 QString("%1").arg(index),
                 this->ui->tblConfig->item(index, 0)==NULL?"0":this->ui->tblConfig->item(index, 0)->text()/*flags*/,
                 this->ui->tblConfig->item(index, 1)==NULL?"nemo":this->ui->tblConfig->item(index, 1)->text()/*name*/,
                 this->ui->tblConfig->item(index, 2)==NULL?"-1":this->ui->tblConfig->item(index, 2)->text()/*dev*/,
                 this->ui->tblConfig->item(index, 3)==NULL?"-1":this->ui->tblConfig->item(index, 3)->text(),/*pin*/
                 this->ui->tblConfig->item(index, 4)==NULL?"-1":this->ui->tblConfig->item(index, 4)->text(),/*X*/
                 this->ui->tblConfig->item(index, 5)==NULL?"-1":this->ui->tblConfig->item(index, 5)->text());/*Y*/
        line = line.arg(
                 this->ui->tblConfig->item(index, 6)==NULL?"0":this->ui->tblConfig->item(index, 6)->text(),/*portion*/
                 this->ui->tblConfig->item(index, 7)==NULL?"0":this->ui->tblConfig->item(index, 7)->text(),/*programm*/
                 this->ui->tblConfig->item(index, 8)==NULL?"0":this->ui->tblConfig->item(index, 8)->text(),/*min*/
                 this->ui->tblConfig->item(index, 9)==NULL?"0":this->ui->tblConfig->item(index, 9)->text(), /*max*/
                 QString("2")/*airtime*/,
                 QString(this->ui->tblConfig->item(index, 12) && this->ui->tblConfig->item(index, 12)->text()==QString::fromLocal8Bit("льём")?"0":"1"),/*watering state*/
                 this->ui->tblConfig->item(index, 11)==NULL?"0":this->ui->tblConfig->item(index, 11)->text(),/*enabled*/
                 QString("0"));//this->ui->tblConfig->item(index, 8)->text().toInt()//
        qDebug()<<"line "<<line;
        port->write(line.toLatin1());
    } else {

    }
}

/**
 *@obsolete
*/
void MakeConfig::sendPlantName(int index)
{
    if (index >= this->ui->tblConfig->rowCount() || this->ui->tblConfig->item(index, 0)->text().length() < 1) {
        ui->statusBar->showMessage(QString::fromLocal8Bit("Отправка конфигурации завершена"));
        state = stNone;
    //    qDebug()<<"state=0, ln334";
        return;
    }

    QString line(QString::fromLocal8Bit("set name %1 %2\r\n").arg(index,2,10,QChar('0')).arg(this->ui->tblConfig->item(index, 0)->text().mid(0, 15)));
    qDebug()<< line;
    port->write(line.toLatin1());
}


void MakeConfig::on_btSave_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.ini)"));
    QSettings settings(fileName/*QString("configs/sensors.ini")*/, QSettings::IniFormat);
    int i=0;
    qDebug()<<fileName;
    while (i < this->ui->tblConfig->rowCount() && ui->tblConfig->item(i, 0) && ui->tblConfig->item(i, 0)->text().length()) {
        QString key;
        qDebug()<<"row "<<i;
        key = key.sprintf("pot_%d/flags", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,0)?this->ui->tblConfig->item(i,0)->text():"-");
        key = key.sprintf("pot_%d/name", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,1)?this->ui->tblConfig->item(i,1)->text():"-");
        key = key.sprintf("pot_%d/chip", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,2)?this->ui->tblConfig->item(i,2)->text():"-");
        key = key.sprintf("pot_%d/pin", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,3)?this->ui->tblConfig->item(i,3)->text():"-");
        key = key.sprintf("pot_%d/X", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,4)?this->ui->tblConfig->item(i,4)->text():"-");
        key = key.sprintf("pot_%d/Y", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,5)?this->ui->tblConfig->item(i,5)->text():"-");
        key = key.sprintf("pot_%d/dose", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,6)?this->ui->tblConfig->item(i,6)->text():"-");
        key = key.sprintf("pot_%d/pgm", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,7)?this->ui->tblConfig->item(i,7)->text():"-");
        key = key.sprintf("pot_%d/min", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,8)?this->ui->tblConfig->item(i,8)->text():"-");
        key = key.sprintf("pot_%d/max", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,9)?this->ui->tblConfig->item(i,9)->text():"-");
        key = key.sprintf("pot_%d/isOn", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,11)?this->ui->tblConfig->item(i,11)->text():"-");
        ++i;
    }
    settings.setValue("global/count", i);

    settings.setValue("global/sz_x", ui->spinX->maximum());
    settings.setValue("global/sz_y", ui->spinY->maximum());
    ui->statusBar->showMessage(QString::fromLocal8Bit("Конфигурация сохранена локально."));
}


void MakeConfig::on_btOpen_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.ini)"));
    QSettings settings(fileName/*QString("configs/sensors.ini")*/, QSettings::IniFormat);
    int i=-1, n=0;
    n = settings.value("global/sz_x", 0).toInt();
    ui->spinX->setMaximum(n);
    n = settings.value("global/sz_y", 0).toInt();
    ui->spinY->setMaximum(n);
    n = settings.value("global/count", 0).toInt() + 1;
    this->ui->tblConfig->setRowCount(n);

    pots_data.clear();

    while ( ++i < n) {
        PlantData pd;
        QString key, val;
        key = key.sprintf("pot_%d/name", i);
        val = settings.value(key, "").toString();
        pd.name = val;
        QTableWidgetItem* item = ui->tblConfig->item(i, 1);

        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 1, item);
        }
        key = key.sprintf("pot_%d/flags", i);
        val = settings.value(key, "").toString();
        item = ui->tblConfig->item(i, 0);
        pd.flags = val.toInt();

        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 0, item);
        }
        key = key.sprintf("pot_%d/pgm", i);
        val = settings.value(key, "").toString();
        pd.pgm = val.toInt();
        item = ui->tblConfig->item(i, 7);

        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 7, item);
        }
        key = key.sprintf("pot_%d/chip", i);
        val = settings.value(key, "").toString();
        pd.chip = val.toInt();
        item = ui->tblConfig->item(i, 2);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 2, item);
        }
        key = key.sprintf("pot_%d/pin", i);
        val = settings.value(key, "").toString();
        pd.pin = val.toInt();
        item = ui->tblConfig->item(i, 3);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 3, item);
        }

        key = key.sprintf("pot_%d/X", i);
        val = settings.value(key, "").toString();
        pd.X = val.toInt();
        item = ui->tblConfig->item(i, 4);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 4, item);
        }

        key = key.sprintf("pot_%d/Y", i);
        val = settings.value(key, "").toString();
        pd.Y = val.toInt();
        item = ui->tblConfig->item(i, 5);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 5, item);
        }

        key = key.sprintf("pot_%d/dose", i);
        val = settings.value(key, "").toString();
        pd.portion = val.toInt();
        item = ui->tblConfig->item(i, 6);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 6, item);
        }

        key = key.sprintf("pot_%d/min", i);
        val = settings.value(key, "").toString();
        pd.min = val.toInt();
        item = ui->tblConfig->item(i, 8);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 8, item);
        }

        key = key.sprintf("pot_%d/max", i);
        val = settings.value(key, "").toString();
        pd.max = val.toInt();
        item = ui->tblConfig->item(i, 9);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 9, item);
        }

        key = key.sprintf("pot_%d/isOn", i);
        val = settings.value(key, "").toString();
        pd.en = val.toInt();
        item = ui->tblConfig->item(i, 11);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 11, item);
        }
        pd.index = i;
        pots_data.push_back(pd);
    }//while
}

void MakeConfig::on_btnGetVals_clicked()
{
    ui->statusBar->showMessage(QString::fromLocal8Bit("Получение текущего состояния поливалки..."));
    state = stReadCurState;
    QMutexLocker ml(mtx);
    buf.clear();
    for (int i = 0; i < ui->tblConfig->rowCount(); ++i) {
        QTableWidgetItem*item = ui->tblConfig->item(i, 10);
        if (!item) {
            item = new QTableWidgetItem("?");
            ui->tblConfig->setItem(i, 10, item);
        }
    }
    port->write("iic list;\r\n");
}

void MakeConfig::on_btObserve_clicked()
{
    ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск устройств..."));
    this->scanner = new QProcess(this);
    QString program("/usr/bin/hcitool");
    QStringList arguments;
    arguments << "scan";
    QObject::connect(this->scanner, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onScanFinished(int,QProcess::ExitStatus)));
    this->scanner->setProcessChannelMode(QProcess::MergedChannels);
    this->scanner->start(program, arguments);
}

void MakeConfig::onScanFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
//    qDebug()<<"scan finished.";
    ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск завершён"));
    QString output(this->scanner->readAllStandardOutput());

    delete this->scanner;
    this->scanner = NULL;

    QStringList devs = output.split(QChar('\n'));
    if (devs.count() < 2 ) {
//        qDebug()<<"no devices found";
        ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск устройств не дал результатов"));
        return;
    }
    QRegExp re("\\s*([0-9a-z\\:]+)\\s*(\\w+)", Qt::CaseInsensitive);
    this->ui->cmbDevice->clear();
    for (int i = 1; i < devs.count(); ++i) {
        QString s = devs.at(i);
        if (re.indexIn(s) > -1) {
            this->ui->cmbDevice->addItem(re.cap(2).append(" [").append(re.cap(1)).append("]"), re.cap(1));
        }
    }//for i
    if (this->ui->cmbDevice->count() > 0) {
        this->ui->btnConnect->setEnabled(true);
        ui->cmbDevice->setEnabled(true);
    }
    ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск завершён. Выберите устройство для связи."));
}

void MakeConfig::on_btnConnect_clicked()
{
    QString address = ui->cmbDevice->itemData((ui->cmbDevice->currentIndex())).toString();
    ui->statusBar->showMessage(QString::fromLocal8Bit("Соединяемся с %1").arg(address));
    if (this->port) {
        port->disconnect();
        port->close();
        delete port;
        port = NULL;
    }

    QString cmd = QString("rfcomm unbind all; rfcomm bind rfcomm0 %1").arg(address);

    system(cmd.toLocal8Bit());


    PortSettings ps;
    ps.BaudRate = BAUD38400;
    ps.DataBits = DATA_8;
    ps.Parity = PAR_NONE;
    ps.StopBits = STOP_1;
    ps.FlowControl = FLOW_OFF;
    ps.Timeout_Millisec = 5;
    //qDebug()<<address;
    this->port = new QextSerialPort("/dev/rfcomm0", ps, QextSerialPort::EventDriven);
    if(!this->port->open(QIODevice::ReadWrite|QIODevice::Text)) {
        QMessageBox::critical(this, "Error", this->port->errorString());
        delete this->port;
        this->port= NULL;
        this->ui->cmbDevice->setEnabled(true);
        this->ui->btnConnect->setEnabled(true);
        this->ui->btDetect->setEnabled(false);
        this->ui->btDetect2->setEnabled(false);
        this->ui->btnGetVals->setEnabled(false);
        this->ui->btSend->setEnabled(false);
        this->ui->btApplyDetected->setEnabled(false);
        this->ui->btSave->setEnabled(false);
        return;
    } else {
        //this->ui->sbStatus->showMessage(QString::fromLocal8Bit("Подключено к %1(%2)").arg(ui->cmbDevice->currentText()).arg(ui->cmbDevice->itemData((ui->cmbDevice->currentIndex())).toString()));
        QObject::connect(this->port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
        this->ui->cmbDevice->setEnabled(true);
        this->ui->btnConnect->setEnabled(true);
        this->ui->btDetect->setEnabled(true);
        this->ui->btDetect2->setEnabled(true);
        this->ui->btnGetVals->setEnabled(true);
        this->ui->btSend->setEnabled(true);
        this->ui->btApplyDetected->setEnabled(true);
        this->ui->btSave->setEnabled(true);
        ui->statusBar->showMessage(QString::fromLocal8Bit("Соединено с %1").arg(ui->cmbDevice->currentText()));
        this->mnu_onLoadFromDev();
    }
}

void MakeConfig::onSetup()
{
    GlobalStateDialog*dlg = new GlobalStateDialog(this);
    QObject::disconnect(this->port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    dlg->setPort(this->port);
    dlg->exec();
    QObject::connect(this->port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    delete dlg;
}

void MakeConfig::on_btnSort_clicked()
{
    int rows = -1;
    std::vector<PlantData> lines;
    PlantData v;
#if 0
    while (++rows < ui->tblConfig->rowCount()) {
        if (ui->tblConfig->item(rows, 0) && ui->tblConfig->item(rows,0)->text().length() > 0) {
            v.chip  = this->ui->tblConfig->item(rows, 1)->text().toInt();//chip
            v.pin   = this->ui->tblConfig->item(rows, 2)->text().toInt();//pin
            v.doser = this->ui->tblConfig->item(rows, 3)->text().toInt();//doser
            v.bowl  = this->ui->tblConfig->item(rows, 4)->text().toInt(); // cup
            v.portion = this->ui->tblConfig->item(rows, 5)->text().toInt();//portion
            v.min = this->ui->tblConfig->item(rows, 6)->text().toInt();//min sensor val
            v.max = this->ui->tblConfig->item(rows, 7)->text().toInt();//max sensor val
            v.daymax = this->ui->tblConfig->item(rows, 8)->text().toInt(); //day max portion
            v.name = this->ui->tblConfig->item(rows, 0)->text();
            lines.push_back(v);
        } else {
            break;
        }
    }
    //ui->tblConfig->setRowCount(rows);
    std::sort(lines.begin(), lines.end());
    ui->tblConfig->setRowCount(0);
    ui->tblConfig->setRowCount(rows);
    QTableWidgetItem*item;
    for (int i = 0; i < lines.size(); ++i) {
        PlantData pd = lines.at(i);

        SETITEM(i, 0, pd.name);
        SETITEM(i, 1, INT2STR(pd.chip));
        SETITEM(i, 2, INT2STR(pd.pin));
        SETITEM(i, 3, INT2STR(pd.doser));
        SETITEM(i, 4, INT2STR(pd.bowl));
        SETITEM(i, 5, INT2STR(pd.portion));
        SETITEM(i, 6, INT2STR(pd.min));
        SETITEM(i, 7, INT2STR(pd.max));
        SETITEM(i, 8, INT2STR(pd.daymax));
        SETITEM(i, 9, QString(""));
        SETITEM(i, 10, QString(""));
        SETITEM(i, 11, QString(""));
    }//for i
    //ui->tblConfig->sortItems(4,Qt::SortOrder);
#endif
}

void MakeConfig::mnu_onLoadFromDev()
{
    QMutexLocker mtxlock(mtx);
    ui->statusBar->showMessage(QString::fromLocal8Bit("Получение текущего состояния поливалки..."));
    state = stReadConfig;
    port->flush();
    buf.clear();
    last_index = -1;
    pots_total = -1;
    this->pots_data.clear();
    qDebug()<<"wrote bytes:"<<port->write("pot get count;\r\n");
//    port->flush();
}

void MakeConfig::on_mnuAddRow()
{
    this->ui->tblConfig->setRowCount(ui->tblConfig->rowCount() + 1);
}

void MakeConfig::on_mnuTestWatering()
{
    QString cmd =QString::fromLocal8Bit("pipi %1,%2,10;\r\n").arg(this->ui->tblConfig->item(ui->tblConfig->currentRow(), 4)->text().toInt()).arg(this->ui->tblConfig->item(ui->tblConfig->currentRow(), 5)->text().toInt());
    qDebug()<<cmd;
    this->port->write(cmd.toLocal8Bit());
    //this->port->flush();
}
//mnuTestWatering

void MakeConfig::on_btnTestWater_clicked()
{
    if (!this->port || !port->isOpen()) return;
    QTableWidgetItem*item = ui->tblConfig->item(ui->tblConfig->currentRow(), 4);
    if (!item) return;
    int X = item->text().toInt();
    item = ui->tblConfig->item(ui->tblConfig->currentRow(), 5);
    if (!item) return;
    int Y = item->text().toInt();
    //qDebug()<<item;
    char cmd[32] = {0};
    state = stNone;
    sprintf(cmd, "pipi %d,%d,10;\r\n", X, Y);
    qDebug()<<cmd;
    this->port->write(cmd);
}

void MakeConfig::on_btnUp_clicked()
{
    if (!port || !port->isOpen()) return;
    state = stNone;
    port->write("U;\r\n");
}

void MakeConfig::on_btnDown_clicked()
{
    if (!port || !port->isOpen()) return;
    state = stNone;
    port->write("D;\r\n");
}

void MakeConfig::on_btnGo_clicked()
{
    if (!port || !port->isOpen()) return;
    QTextStream stm;
    QString cmd;
    stm.setString(&cmd);
    stm<<"G";
    if (ui->chkRestart->isChecked()) stm<<"!";
    stm<<ui->spinX->value()<<","<<ui->spinY->value()<<";\r\n";
    state = stNone;
    qDebug()<<cmd;
    port->write(cmd.toLocal8Bit());
}

void MakeConfig::on_btnPipi_clicked()
{
    if (!port || !port->isOpen()) return;
    QString cmd("pipi %1,%2,%3;\r\n");
    cmd = cmd.arg(ui->spinX->value()).arg(ui->spinY->value()).arg(ui->spinML->value());
    state = stNone;
    port->write(cmd.toLocal8Bit());
}

void MakeConfig::on_btnTryAll_clicked()
{
    if (!port || !port->isOpen()) return;
    state = stNone;
    port->write("try all;\r\n");

}

void MakeConfig::on_edCmd_returnPressed()
{
    if (!port || !port->isOpen()) return;
    state = stNone;
    port->write(ui->edCmd->text().append("\r\n").toLocal8Bit());
}

void MakeConfig::on_actWST_clicked()
{
    WaterStorages*dlg = new WaterStorages(this);
    QObject::disconnect(this->port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    dlg->setPort(this->port);
    dlg->exec();
    QObject::connect(this->port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    delete dlg;
}

#define GETINTVAL(r, c) ((ui->tblConfig->item(r, c)==NULL || ui->tblConfig->item(r, c)->text().isEmpty())?0:ui->tblConfig->item(r, c)->text().toInt())
void MakeConfig::showVisualCfg()
{
    VisualConfig*dlg = new VisualConfig(this);
    dlg->exec();
    return;
    dlg->setPort(port);
    pots_data.clear();
    QTableWidgetItem* item;

    for (int i = 0; i < ui->tblConfig->rowCount(); ++i) {

        PlantData pd;
        pd.index = i;
        pd.flags = GETINTVAL(i, 0);
        pd.name = ui->tblConfig->item(i, 1)==NULL?"-":ui->tblConfig->item(i, 1)->text();
        if (pd.name.isEmpty()) continue;
        pd.chip = GETINTVAL(i, 2);
        pd.pin = GETINTVAL(i, 3);
        pd.X = GETINTVAL(i, 4);
        pd.Y = GETINTVAL(i, 5);
        pd.portion = GETINTVAL(i, 6);
        pd.pgm = GETINTVAL(i, 7);
        pd.min = GETINTVAL(i, 8);
        pd.max = GETINTVAL(i, 9);
        pd.en = GETINTVAL(i, 11);
        pots_data.push_back(pd);
    }

    dlg->setPotsData(&pots_data);
    dlg->setWaterDoserSizes(ui->spinX->maximum(), ui->spinY->maximum());
    dlg->exec();
    ///TODO: REFRESH ui->tblData FROM pots_data!!! -- DONE
    ui->tblConfig->setRowCount(0);
    ui->tblConfig->setRowCount(pots_data.count());

    for (int i = 0; i < pots_data.count(); ++i) {

        PlantData pd = pots_data.at(i);
        if (pd.name.isEmpty()) {
            continue;
        }
        SETITEM(i, 0, INT2STR(pd.flags));
        SETITEM(i, 1, pd.name);
        SETITEM(i, 2, INT2STR(pd.chip));
        SETITEM(i, 3, INT2STR(pd.pin));
        SETITEM(i, 4, INT2STR(pd.X));
        SETITEM(i, 5, INT2STR(pd.Y));
        SETITEM(i, 6, INT2STR(pd.portion));
        SETITEM(i, 7, INT2STR(pd.pgm));
        SETITEM(i, 8, INT2STR(pd.min));
        SETITEM(i, 9, INT2STR(pd.max));
        SETITEM(i, 10, QString(""));
        SETITEM(i, 11, INT2STR(pd.en));
        SETITEM(i, 12, QString(""));
        SETITEM(i, 13, QString(""));
    }
    delete dlg;
}
