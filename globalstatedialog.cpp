#include "globalstatedialog.h"
#include "ui_globalstatedialog.h"
#include <QDateTime>
#include <QDate>
#include <QTime>

#include<QDebug>

GlobalStateDialog::GlobalStateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GlobalStateDialog)
{
    ui->setupUi(this);
    this->port = NULL;
    this->state = gstNone;
    buf.clear();
    mtx = new QMutex();
}

GlobalStateDialog::~GlobalStateDialog()
{
    delete ui;
}

void GlobalStateDialog::setPort(QextSerialPort *port)
{
    this->port = port;
    if (this->port->isOpen()) {
        QObject::connect(this->port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
        updateData();
        /*this->state = gstReadTime;
        buf.clear();
        port->write("time get;\r\n");*/
    }
}

void GlobalStateDialog::updateData()
{
    this->port = port;
    if (this->port->isOpen()) {
        state = gstReadCfg;
        port->write("cfg get;\r\n");
    }
}

void GlobalStateDialog::onDataAvailable()
{
    QMutexLocker ml(mtx);
    buf.append(port->readAll());
    qDebug()<<"buf:"<<buf;
    if (!buf.contains(';')) return;

    if ( state == gstReadTime ) {
        buf.chop(1);
        ui->edTime->setText(buf);
        /*QRegExp timeRe("^(\d+)\.(\d+)\.(\d+)\s+(\w+)\s+(\d+)\:(\d+)\:(\d+)$");
        if (timeRe.indexIn(buf.trimmed()) > -1) {
                QString nums = il.cap(1), name = il.cap(2);
                nums = nums.replace(QRegExp("\\s+"), "");
        }*/
    } else if (state == gstReadCfg) {
        QStringList data = buf.split(",");
        saved_flags = data[0].toInt();
        qDebug()<<"flags="<<saved_flags;
        //buf: "0,1,500,100,100,900,2100,900,2100,3;"
        ui->chkIsRun->setChecked(saved_flags & 1);
        ui->edPotsCount->setText(data[1]);
        //ui->rdStop->setChecked(flags & 1);
        ui->edPwrOn->setText(data[2]);
        ui->edSensorInit->setText(data[3]);
        ui->edRead->setText(data[4]);
        int val = data[5].toInt(), val1;
        val1 = val%100;
        val /= 100;
        ui->edWDTimeFrom->setText(QString("%1").arg(val));
        ui->edWDTimeFromM->setText(QString("%1").arg(val1));
        val = data[6].toInt();
        val1 = val%100;
        val /= 100;
        ui->edWDTimeTo->setText(QString("%1").arg(val));
        ui->edWDTimeToM->setText(QString("%1").arg(val1));
        val = data[7].toInt();
        val1 = val%100;
        val /= 100;
        ui->edWETimeFrom->setText(QString("%1").arg(val));
        ui->edWETimeFromM->setText(QString("%1").arg(val1));
        val = data[8].toInt();
        val1 = val%100;
        val /= 100;
        ui->edWETimeTo->setText(QString("%1").arg(val));
        ui->edWETimeToM->setText(QString("%1").arg(val1));

        ui->edMeasures->setText(data[9]);
        ui->edCheckTime->setText(data[10]);
        /*ui->spinUpPos->setValue(data[10].toInt());
        data[11] = data[11].trimmed();
        data[11].chop(1);
        qDebug()<<data[11]<<" "<<data[11].toInt();
        ui->spinDownPos->setValue(data[11].toInt());
        */
        this->state = gstNone;
        buf.clear();
        return;
        //QTextStream stm;
        //stm.setString(&buf, QIODevice::ReadOnly);
    } else if (state == gstReadState) {
       /* buf.append(port->readAll());
        qDebug()<<buf;
        if (!buf.contains(";;")) return;
        QStringList ls = buf.split("\n");
        if (ls.count() < 3) {
            qDebug()<<"not enough data=(";
            state = gstNone;
            return;
        }
        QTextStream st;
        QString str = ls.at(0);
        st.setString(&str);
        int h,mi,s,d,mo,dow;
        st>>h;
        st>>mi;
        st>>s;
        st>>d;
        st>>mo;
        st>>dow;
        char*days[] = {"Пн","Вт","Ср","Чт","Пт","Сб","Вс"};
        ui->edTime->setText(QString::fromLocal8Bit("%1:%2:%3 %4.%5 %6").arg(h).arg(mi).arg(s).arg(d).arg(mo).arg(QString::fromLocal8Bit(days[ dow ])));
        str = ls.at(1);
        st.setString(&str);
        st>>h;
        ui->rdStart->setChecked(h==1);
        ui->rdStop->setChecked(h==0);
        st>>h;
        ui->lblNight->setText(QString::fromLocal8Bit(h==1?"Ночь":"День"));
        st>>h;
        ui->edNightWDFrom->setText(QString("%1").arg(h));
        st>>h;
        ui->edNightWDTo->setText(QString("%1").arg(h));
        st>>h;
        ui->edNightWeFrom->setText(QString("%1").arg(h));
        st>>h;
        ui->edNightWETo->setText(QString("%1").arg(h));
        ui->lsvErrors->clear();
        ui->lsvErrors->addItem(ls.at(2));
        str = ls.at(2);
        st.setString(&str);
        st.setIntegerBase(16);
        d = 0;
        qDebug()<<"parse errors str"<<str;
        int val = 0;
        do {
            qDebug()<<"base:"<<st.integerBase();
            st>>val;
            qDebug()<<val<< (val>>4);
            if (val == 0) break;
            if ( (val>>4) < 10) {
                switch (val&0x0F) {
                    case 0x01: ui->lsvErrors->addItem(QString::fromLocal8Bit("Насос #%1: нет данных от счётчика жидкости").arg(val>>4));
                        break;
                    case 0x00:
                        ui->lsvErrors->addItem(QString::fromLocal8Bit("Насос #%1 всё хорошо").arg(val>>4));
                        break;
                    default:
                        ui->lsvErrors->addItem(QString::fromLocal8Bit("Насос #%1: неизвестное состояние %2").arg(val>>4).arg(val&0x0F));
                        break;
                }
            } else {
                switch (val&0x0F) {
                    case 0x01: ui->lsvErrors->addItem(QString::fromLocal8Bit("Дозатор #%1: не совпадает кол-во чашек").arg((val>>4)-10));
                        break;
                    case 0x03:
                        ui->lsvErrors->addItem(QString::fromLocal8Bit("Дозатор #%1: ошибочный номер чашки").arg((val>>4)-10));
                        break;
                    case 0x04:
                        ui->lsvErrors->addItem(QString::fromLocal8Bit("Дозатор #%1: промах мимо чашки").arg((val>>4) -10));
                        break;
                    case 0x05:
                        ui->lsvErrors->addItem(QString::fromLocal8Bit("Дозатор #%1: неисправимая ошибка").arg((val>>4) -10));
                        break;
                    case 0:
                        ui->lsvErrors->addItem(QString::fromLocal8Bit("Дозатор #%1: всё хорошо").arg((val>>4) -10));
                        break;
                }
            }
        } while (val >= 0x10 && d++ < 4);

        buf.clear();
        */
    } else if (state == gstSendCommand) {
        buf.append(port->readAll());
        if (buf.contains("\n")) {
            buf.clear();
            state = gstReadState;
            port->write("getstate\r\n");
        }
    }
}


/*void GlobalStateDialog::on_btnSetTime_clicked()
{
    QTextStream stm;
    QString cmd;
    stm.setString(&cmd);
    stm<<"time set ";
    QDateTime dt = QDateTime::currentDateTime();
    dt.addSecs(5);
    QTime time = dt.time();
    stm<<time.hour()<<':'<<time.minute()<<':'<<time.second()<<' ';
    QDate date = dt.date();
    stm<<date.day()<<'.'<<date.month()<<' '<<date.dayOfWeek()<<";\r\n";
    qDebug()<<cmd;
    if (port && port->isOpen()) {
        state = gstSendCommand;
        port->write(cmd.toLocal8Bit());
    }
}*/

void GlobalStateDialog::on_btRefresh_clicked()
{
    QMutexLocker ml(mtx);
    if (port){
        buf.clear();
        state = gstReadCfg;
        port->write("cfg get;\r\n");
    }
}

void GlobalStateDialog::on_btTestPos_clicked()
{
   /* QMutexLocker ml(mtx);
    if (port){
        buf.clear();
        state = gstNone;
        QString cmd("S%1;\r\n");
        port->write(cmd.arg(ui->spinUpPos->value()).toLocal8Bit());
    }*/
}

void GlobalStateDialog::on_btnServoTest_clicked()
{
 /*   QMutexLocker ml(mtx);
    buf.clear();
    if (port){
        state = gstNone;
        QString cmd("S%1;\r\n");
        port->write(cmd.arg(ui->spinDownPos->value()).toLocal8Bit());
    }*/
}

void GlobalStateDialog::on_btnGetTime_clicked()
{
    QMutexLocker ml(mtx);
    if (port){
        state = gstReadTime;
        buf.clear();
        port->write("time get;\r\n");
    }
}

void GlobalStateDialog::on_btnApplyAll_clicked()
{
    QString cmd;
    QTextStream stm;
    stm.setString(&cmd);
    stm<<"cfg set ";
    stm<<(ui->chkIsRun->isChecked()?(saved_flags|1):(saved_flags&0xFE));
    stm<<",";
    stm<<ui->edPotsCount->text();
    stm<<",";
    stm<<ui->edPwrOn->text();
    stm<<",";
    stm<<ui->edSensorInit->text();
    stm<<",";
    stm<<ui->edRead->text();
    stm<<",";
    stm<<(ui->edWDTimeFrom->text().toInt()*100+ui->edWDTimeFromM->text().toInt());
    stm<<",";
    stm<<(ui->edWDTimeTo->text().toInt()*100+ui->edWDTimeToM->text().toInt());
    stm<<",";
    stm<<(ui->edWETimeFrom->text().toInt()*100+ui->edWETimeFromM->text().toInt());
    stm<<",";
    stm<<(ui->edWETimeTo->text().toInt()*100+ui->edWETimeToM->text().toInt());
    stm<<",";
    stm<<ui->edMeasures->text();
    stm<<",";
    stm<<ui->edCheckTime->text();
    stm<<";\r\n";

/*    stm<<ui->edCheckTime->text();
    stm<<",";
    stm<<ui->spinUpPos->value();
    stm<<",";
    stm<<ui->spinDownPos->value();
    stm<<";\r\n";*/
    qDebug()<<cmd;
    QMutexLocker ml(mtx);
    if (port){
        state = gstNone;
        buf.clear();
        port->write(cmd.toLocal8Bit());
    }
}

void GlobalStateDialog::on_btnSetTime_clicked()
{
    QTextStream stm;
    QString cmd;
    stm.setString(&cmd);
    stm<<"time set ";
    QDateTime now = QDateTime::currentDateTime();
    stm<<now.time().hour();
    stm<<":"<<now.time().minute();
    stm<<":"<<now.time().second();
    stm<<" "<<now.date().day();
    stm<<"."<<now.date().month();
    stm<<"."<<now.date().year();
    stm<<" "<<now.date().dayOfWeek()<<";\r\n";
    qDebug()<<cmd;
    if (port) {
        state = gstNone;
        buf.clear();
        port->write(cmd.toLocal8Bit());
    }
}
