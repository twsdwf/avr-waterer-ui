#include "visualconfig.h"
#include "ui_visualconfig.h"
#include<QDebug>
#include<QLabel>
#include "data_structs.h"
#include "connectdlg.h"
#include <QFileDialog>
#include <QSettings>
#include "globalstatedialog.h"
#include "waterstorages.h"

VisualConfig::VisualConfig(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VisualConfig), b_new(false)
{
    ui->setupUi(this);
    ui->txtLog->setReadOnly(true);

    port = NULL;
    lay = ui->gridXY;
    state = vcstNone;
    mtx = new QMutex;
    this->pots_data = NULL;
    sigmap = new QSignalMapper(this);
    chip_sigmap = new QSignalMapper(this);
    cur_pot_index = -1;
    QBrush windowText(Qt::black),button(Qt::darkGreen), light(Qt::darkGreen), dark(Qt::darkGreen), mid(Qt::darkGreen),
            text(Qt::black),
            bright_text(Qt::lightGray),
            base(Qt::darkGreen), window(Qt::darkGreen);

    btn_pal = QPalette(ui->btnApply->palette());
    btn_pal_checked = QPalette(windowText, button, light,dark, mid, text, bright_text,base, window);
    /*    QPalette(const QColor &windowText, const QColor &window, const QColor &light,
             const QColor &dark, const QColor &mid, const QColor &text, const QColor &base);*/
    chip_addr_label_palette = QPalette(Qt::white, Qt::black, Qt::black, Qt::black, Qt::black, Qt::white, Qt::black);
    pinValueEdit = NULL;
    __setExchButtonsState(false);
    connect(ui->actExit, SIGNAL(triggered()), this, SLOT(close()));
}

VisualConfig::~VisualConfig()
{
    disconnect(sigmap);
    disconnect(port);
   /* for (int i=0;i< xy.count();++i) {
        disconnect(xy[i]);
        delete xy[i];
    }
    xy.clear();
    //QMap<int, QMap<int, QLabel*>*> chip_view;
    for (QMap<int, QMap<int, QLabel*>*>::iterator it = chip_view.begin(); it!=chip_view.end(); ++it) {
        for (QMap<int, QLabel*>::iterator it2 = it.value()->begin(); it2 != it.value()->end();++it2) {
        }
    }*/
    delete ui;
}

void VisualConfig::setPort(QextSerialPort *port)
{
    this->port = port;
    bool b = this->port && this->port->isOpen();
    ui->btReadSensor->setVisible(b);
    ui->btReadSensor_2->setVisible(b);
    ui->btReadSensor_3->setVisible(b);
    ui->btnDown->setVisible(b);
    ui->btnGo->setVisible(b);
    ui->btnUp->setVisible(b);
    ui->chkRestart->setVisible(b);
    ui->btnPipi->setVisible(b);
    ui->btnTryAll->setVisible(b);

    if (b) {
        QObject::connect(this->port, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
        state = vcstReadSize;
        qDebug()<<"WSZ send;";
        this->port->write("WSZ;\r\n");
    }
}

void VisualConfig::setWaterDoserSizes(int _x, int _y)
{
    this->szx = _x;
    this->szy = _y;
    qDebug()<<"setWaterDoserSizes("<<_x<<","<<_y<<")";
    /*QList<QAbstractButton *> rms = btngrp->buttons();
    for (int i = 0; i< rms.count(); ++i) {
        btngrp->removeButton(rms[i]);
    }*/
    QPushButton*btn;
    QLabel*lbl;
    xy.clear();

    for (int x = 0 ; x < szx; ++x ) {
        lbl = new QLabel(QString("%1").arg(x));
        lbl->setMaximumSize(32,32);
        lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        lay->addWidget(lbl, x+1, 0);
        for (int y = 0; y < szy; ++ y) {
            if (x == 0) {
                lbl = new QLabel(QString("%1").arg(y));
                lbl->setMaximumSize(32,32);
                lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                lay->addWidget(lbl, 0, y + 1);
            }
            btn = new QPushButton(QString(__isXYused(x,y)?"*":""));
            //btn->setCheckable(true);
            btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            btn->setMaximumSize(32, 32);
            btn->setMinimumSize(32, 32);
            btn->setCheckable(true);
            btn->setPalette(btn_pal);
            lay->addWidget(btn, x + 1, y + 1);
            sigmap->setMapping(btn, 100 * x + y);
            connect(btn, SIGNAL(clicked()), sigmap, SLOT(map()));
            xy.push_back(btn);
        }
    }
    connect(sigmap, SIGNAL(mapped(int)),this, SLOT(xy_clicked(int)));
}

void VisualConfig::setPotsData(QVector<PlantData> *data)
{
    this->pots_data = data;

    if (data == NULL || data->count() == 0 ) {
        return;
    }
    QMap<int, int> chips;

    for (int i = 0; i < data->count(); ++ i ) {
        PlantData pd = data->at(i);
        if (chips.find(pd.chip) == chips.end() && pd.chip > 0) {
            chips.insert(pd.chip, 1);
        }
    }

    disconnect(chip_sigmap);
    disconnect(SIGNAL(mapped(int)), chip_sigmap);
    if (chip_sigmap) {
        delete chip_sigmap;
    }
    chip_sigmap = new QSignalMapper(this);
    //ui->tabChips->removeTab(0);

    while(ui->tabChips->count()) {
        QWidget*tab = ui->tabChips->widget(0);
        ui->tabChips->removeTab(0);
        delete tab;
    }

    sensor_values.clear();

    for (QMap<int, int>::iterator it = chips.begin(); it!=chips.end();++it) {
        //QMap<int, QMap<int, QPushButton*>* > chip_view;
        QMap<int, QPushButton*>* map = new QMap<int, QPushButton*>;
        map->clear();
        chip_view.insert((int)it.key(), map);
        QMap<int, QLabel*>* chip_sensor_values = new QMap<int, QLabel*>;

        sensor_values.insert(it.key(), chip_sensor_values);

        //qDebug()<<"chip "<<it.key();
        QWidget *tpage = new QWidget;
        QGridLayout*page = new QGridLayout;
        QLabel*chip = new QLabel(QString("%1").arg(it.key()));
        chip->setPalette(chip_addr_label_palette);
        QFont font = chip->font();
        font.setBold(true);
        font.setPointSize(34);
        chip->setFont(font);
        chip->setBackgroundRole(QPalette::Background);
        chip->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        chip->setStyleSheet(QString("QLabel{background:black;color:white;text-align:center;border:3px solid silver;}"));
        page->addWidget(chip, 0, 2, 8, 1, 0);

        for (QVector<PlantData>::iterator it2 = pots_data->begin(); it2!=pots_data->end(); ++it2) {
            //qDebug()<<"test chip "<<it2->chip<<" pin "<<it2->pin;
            if (it2->chip == it.key()) {
                if (map->find(it2->pin) == map->end()) {
                    //qDebug()<<"add label for plant "<<it2->name;
                    QPushButton*btn = new QPushButton(QString("%1 %2").arg(it2->pin, 2,10, QChar(' ')).arg(it2->name));
                    //lbl->setFlat(true);
                    btn->setCheckable(true);
                    chip_sigmap->setMapping(btn, it.key()*100 + it2->pin);
                    connect(btn, SIGNAL(clicked()), chip_sigmap, SLOT(map()));
                    map->insert((int)it2->pin, btn);
                    page->addWidget(btn, it2->pin % 8, (it2->pin / 8 > 0) ? 4 : 0);

                    QLabel*lbl = new QLabel("-");
                    page->addWidget(lbl, it2->pin % 8, (it2->pin / 8 > 0) ? 3 : 1);
                    chip_sensor_values->insert(it2->pin, lbl);
                } else {
                    qDebug()<<"chip "<<it2->chip<<" pin "<<it2->pin<<" found duplicate usage!";
                }
            }//if matched chip address
        }//for it2
        for (int p = 0; p < 16; ++p) {
            if (map->find(p) == map->end()) {
                QPushButton*btn = new QPushButton(QString(" %1 (EMPTY)").arg(p));
                //lbl->setFlat(true);
                btn->setCheckable(true);
                chip_sigmap->setMapping(btn, it.key() * 100 + p);
                connect(btn, SIGNAL(clicked()), chip_sigmap, SLOT(map()));
                map->insert(p, btn);
                page->addWidget(btn, p % 8, (p / 8 > 0) ? 4 : 0);

                QLabel*lbl = new QLabel("-");
                page->addWidget(lbl, p % 8, (p / 8 > 0) ? 3 : 1);
                chip_sensor_values->insert(p, lbl);
            }
        }
        QString head = QString("chip %1").arg(it.key());
        tpage->setLayout(page);
        ui->tabChips->addTab(tpage, head);
    }
    connect(chip_sigmap, SIGNAL(mapped(int)),this, SLOT(pinOfChip_clicked(int)));
}

void VisualConfig::xy_clicked(int id)
{
    int x = id / 100;
    int y = id % 100;

    if (this->pots_data == NULL) {
        qDebug()<<"no pots data";
        return;
    }
    qDebug()<<"xy_clicked("<<x<<","<<y<<") sizes "<<szx<<"x"<<szy;
    __resetState(x * szy + y, -1);
    for (QVector<PlantData>::const_iterator it = pots_data->constBegin(); it != pots_data->constEnd(); ++it) {
        if (it->X == x && it->Y == y) {
            //qDebug()<<"found "<<it->name<<" "<<it->X<<" "<<it->Y;
            this->__setCurrentPot(it);
            return;
        }
    }

    ui->spinX->setValue(x);
    ui->spinY->setValue(y);
    if (!ui->edName->text().isEmpty()) {
        this->__resetState(x*szy+y, -1);
        ui->edChip->setText("-1");
        ui->edPin->setText("-1");
        ui->edName->setText("");
    }
    ui->chkEn->setChecked(false);
}

void VisualConfig::pinOfChip_clicked(int id)
{
    int chip = id / 100;
    int pin  = id % 100;
    //qDebug()<<"pinOfChip_clicked("<<chip<<","<<pin<<");";


    if (this->pots_data == NULL) return;
    b_new = true;
    for (QVector<PlantData>::const_iterator it = pots_data->constBegin(); it != pots_data->constEnd(); ++it) {
        if (it->chip == chip && it->pin == pin) {
            b_new = false;
            __setCurrentPot(it);
            return;
        }
    }

    if (!ui->edName->text().isEmpty()) {
        this->__resetState(-1, id);
        ui->spinX->setValue(0);
        ui->spinY->setValue(0);
        ui->edName->setText("");
    }

    ui->edChip->setText(QString("%1").arg(chip));
    ui->edPin->setText(QString("%1").arg(pin));
    ui->chkEn->setChecked(false);
}

void VisualConfig::dataAvailable()
{
    //qDebug()<<"data:"<<port->readAll();
    QMutexLocker ml(mtx);
    QByteArray data = port->readAll();

    static int dumb_reads = 0;
   /* if (data.length() > 0 && data.contains(';')) {
        QString text = ui->txtLog->toPlainText().trimmed();
        if (text.endsWith(QChar(';'))) {
            text.append("\r\n");
        }
        text.append(data);*/
        ui->txtLog->appendPlainText(data);
   // }
    if (data.length() == 0) {
        if (dumb_reads > 10) {
            port->close();
            disconnect(SIGNAL(readyRead()), this);
            delete port;
            port = NULL;
            __setExchButtonsState(false);
            ui->lbMsg->setText(QString::fromLocal8Bit("Соединение потеряно"));
        } else {
            ++dumb_reads;
        }
        return;
    }
    dumb_reads = 0;
    buf.append(data);
    qDebug()<<buf;
    if (state == vcstNone) {
        buf.clear();
        return;
    } else if (state == vcstReadConfig) {
        if (buf.contains(";")) {
           if (last_index == -1) {
               qDebug()<<"got pot cnt";
               buf = buf.trimmed();
               buf.chop(1);
               pots_total = buf.toInt();
               last_index = 0;
               buf.clear();
               if (pots_data == NULL) {
                   pots_data = new QVector<PlantData>;
               } else {
                   pots_data->clear();
               }
               this->port->write(QString("pot get %1;\r\n").arg(last_index).toLocal8Bit());
               __msg(QString::fromLocal8Bit("Запрос данных %1/%2").arg(last_index).arg(pots_total));
           } else if (buf.length() > 10 && buf.contains(",")) {
               buf = buf.trimmed();
               buf.chop(1);
               QStringList parts = buf.split(",");
               if (parts.length() >=12) {
//pot set <index>,<flags>,<dev>,<pin>,<x>,<y>,<name>,<airTime>,<state>,<enabled>,<ml>,<pgm>,<param1>[,param2][,param3]...;
                   PlantData pd;
                   pd.index = parts[0].toInt();
                   pd.flags = 0;//parts[1].toInt();
                   pd.chip = parts[1].toInt();
                   pd.pin = parts[2].toInt();
                   pd.X = parts[3].toInt();
                   pd.Y = parts[4].toInt();
                   pd.name = parts[5];
                   pd.en = parts[8].toInt();
                   pd.portion = parts[9].toInt();
                   pd.pgm = parts[10].toInt();
                   qDebug()<<"pot index="<<pd.index<<" chip="<<pd.chip<<" pin="<<pd.pin<<" x="<<pd.X<<"y="<<pd.Y
                          <<" name="<<pd.name<<" portion="<<pd.portion<<" en="<<pd.en<<" pgm="<<pd.pgm;
                   if (pd.pgm == 1) {
                       pd.max = parts[11].toInt();
                       pd.daymax = parts[12].toInt();
                       qDebug()<<" barval="<<pd.max<<" daymax="<<pd.daymax;
                   } else if(pd.pgm == 2) {
                        pd.min = parts[11].toInt();
                        pd.max = parts[12].toInt();
                        pd.daymax = parts[13].toInt();
                        qDebug()<<" range:"<<pd.min<<".."<<pd.min<<" daymax="<<pd.daymax;
                   }
                   this->pots_data->push_back(pd);
                   ++last_index;
               } else {
                   qDebug()<<"re-query pot "<<last_index;
               }
               buf.clear();
               if (last_index < pots_total) {
                   qDebug()<<"query pot "<<last_index;
                   __msg(QString::fromLocal8Bit("Запрос данных %1/%2").arg(last_index).arg(pots_total));
                   this->port->write(QString("pot get %1;\r\n").arg(last_index).toLocal8Bit());
                   //this->port->flush();
               } else {
                   this->setPotsData(pots_data);
                   setWaterDoserSizes(szx, szy);
                   /*state = vcstReadSize;
                   __msg("Запрос данных о дозаторе...");
                   this->port->write("WSZ;\r\n");
                   */
               }
           } else {
               buf.clear();
               __msg(QString::fromLocal8Bit("Запрос данных %1/%2").arg(last_index).arg(pots_total));
               this->port->write(QString("pot get %1;\r\n").arg(last_index).toLocal8Bit());
           }
           return;
        } else {
           return;
        }
    } else if (state == vcstReadSize) {
        QRegExp re("^.*(\\d+)\\s*\\,\\s*(\\d+).*");
        qDebug()<<buf;
        if (re.indexIn(buf) > -1) {
            qDebug()<<re.cap(1)<< re.cap(1).toInt()<<re.cap(2)<< re.cap(2).toInt();
            ui->spinX->setMaximum(re.cap(1).toInt());
            ui->spinY->setMaximum(re.cap(2).toInt());
            this->setWaterDoserSizes(re.cap(1).toInt(), re.cap(2).toInt());
            buf.clear();
            state = vcstNone;
            __msg("Данные о поливалке получены");
        } else {
            //qDebug()<<buf<<" : not matched";
        }
    } else if (state == vcstReadPin) {
        QString cleaned = buf.replace(QRegExp("\s"), "");
        qDebug()<<"cleaned:["<<cleaned<<"]";
        QRegExp re("^.*(\\d+),(\\d+)\\,(\\d+);");
        if (re.indexIn(cleaned) > -1) {
            if (re.captureCount() >=3) {
                pinValueEdit->setText(re.cap(3));
                state = vcstNone;
                buf.clear();
            }
        }
    } else if (state == vcstReadCurStateStep1) {
        if (!buf.contains(";;")) return;
        QString s = QString(buf);
        QStringList devs = s.split(QString(";"));
        qDebug()<<"parts:"<<devs.count();
        if (devs.count() < 2) {
            state = vcstNone;
            __msg("Нет датчиков!");
            qDebug()<<"no sensor devices found";
            buf.clear();
            return;
        }
        for (int i=0; i< devs.count();++i) {
            if (devs.at(i).length() < 5) {
                continue;
            }//if len
            qDebug()<<"string:"<<devs[i];
            QRegExp re("^(\\d{2})\\:(\\d{1,4})\\,(\\d{1,4})\\,(\\d{1,4})\\,(\\d{1,4})\\,(\\d{1,4})\\,(\\d{1,4})\\,(\\d{1,4})\\,(\\d{1,4})\\,(\\d{1,4})\\,(\\d{1,4})\\,(\\d{1,4})\\,(\\d{1,4})\\,(\\d{1,4})\\,(\\d{1,4})\\,(\\d{1,4})\\,(\\d{1,4})");
            if (re.indexIn(devs[i]) > -1) {
                int chip = re.cap(1).toInt();
                for(int pin=0;pin<16;++pin) {
                        QMap<int, QMap<int, QLabel*>*>::iterator itc = sensor_values.find(chip);
                        if (itc != sensor_values.end()) {
                            QMap<int, QLabel*>::iterator itp = itc.value()->find(pin);
                            if (itp!= itc.value()->end()) {
                                itp.value()->setText(re.cap(pin+2));
                            } else {
                                qDebug()<<"pin "<<pin<<" not found";
                            }
                        } else {
                            qDebug()<<"chip "<<chip<<" not found";
                        }
                }//for pin
            }//if capped
        }//for i
        /*QString cmd;
        for (int i=0; i< devs.count()-1; ++i ) {
            cmd.append(QString("iic %1,*;").arg(devs[i]));
        }*/
        buf.clear();
        /*port->write(cmd.toLocal8Bit());
        state = vcstReadCurStateStep2;
        __msg("Запрос данных с датчиков...");
        */
        return;
    } else if (state == vcstReadCurStateStep2) {
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
        int chip = parts[0].toInt(), pin = parts[1].toInt();
        QMap<int, QMap<int, QLabel*>*>::iterator itc = sensor_values.find(chip);
        if (itc != sensor_values.end()) {
            QMap<int, QLabel*>::iterator itp = itc.value()->find(pin);
            if (itp!= itc.value()->end()) {
                itp.value()->setText(parts[2]);
            } else {
                qDebug()<<"pin "<<pin<<" not found";
            }
        } else {
            qDebug()<<"chip "<<chip<<" not found";
        }
       /* while (r < this->ui->tblConfig->rowCount()) {
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
        */
        buf = buf.remove(0, buf.indexOf(";") + 1);
    } else if (state == vcstSendConfig) {

        if (buf.contains("OK")) {
            buf.clear();
            if (last_index == pots_data->count()) {
                qDebug()<<"cfg ok, setting pots count...";
                state = vcstSendSensorsCount;
                this->port->write(QString("pot set count %1;\r\n").arg(pots_data->count()).toLatin1());
            } else {
            __msg(QString::fromLocal8Bit("Отправляем строку конфигурации №%1").arg(last_index + 1));
            __sendConfigLine(++last_index);
            }
        } else if (buf.contains("setup()")) {
            buf.clear();
            __sendConfigLine(last_index);
        }
        return;
    } else if (state == vcstSendSensorsCount) {
        if (buf.contains("pots=") || buf.contains("OK;")) {
            buf.clear();
            __msg("Конфигурация отправлена");
            state = vcstNone;
        }
    }
}

bool VisualConfig::__isXYused(int x, int y)
{
    //qDebug()<<"__isXYused:"<<this->pots_data;
    if (this->pots_data == NULL) return false;
    for (QVector<PlantData>::iterator it = pots_data->begin(); it != pots_data->end(); ++it) {
        if (it->X == x && it->Y == y) return true;
    }
    return false;
}


void VisualConfig::__setCurrentPot(QVector<PlantData>::const_iterator& it)
{
    //resetState();
    qDebug()<<"setCurrentPot("<<it->name<< " "<<it->Y<<" "<<it->Y<<")";
    ui->edName->setText(it->name);
    ui->edChip->setText(QString("%1").arg(it->chip));
    ui->edPin->setText(QString("%1").arg(it->pin));
    ui->spinX->setValue(it->X);
    ui->spinY->setValue(it->Y);
    ui->chkEn->setChecked(it->en);
    ui->edML->setText(QString("%1").arg(it->portion));
    ui->edDayMax->setText(QString("%1").arg(it->daymax));
    ui->grpbxPgm1->setVisible(it->pgm == 1);
    ui->grpbxPgm2->setVisible(it->pgm == 2);
    qDebug()<<"pgm:"<<it->pgm;
    ui->rdPgm1->setChecked(it->pgm == 1);
    ui->rdPgm2->setChecked(it->pgm == 2);
    ui->edPgm1Value->setText(QString("%1").arg(it->max));
    ui->edPgm2MinValue->setText(QString("%1").arg(it->min));
    ui->edPgm2MaxValue->setText(QString("%1").arg(it->max));


    int this_btn = it->X * szy + it->Y;
    qDebug()<<"this_btn="<<this_btn;
    for (int i = 0; i < xy.count(); ++i) {
        ((QPushButton*)xy[i])->setChecked(i == this_btn);
        ((QPushButton*)xy[i])->setPalette(i == this_btn ? btn_pal_checked : btn_pal);
    }

    for (QMap<int, QMap<int, QPushButton*>*>::iterator _it = chip_view.begin(); _it!=chip_view.end(); ++_it) {
       for (QMap<int, QPushButton*>::iterator it2 = _it.value()->begin(); it2 != _it.value()->end();++it2) {
            ((QPushButton*)it2.value())->setChecked( _it.key() == it->chip && it2.key() == it->pin );
            ((QPushButton*)it2.value())->setPalette( (_it.key() == it->chip && it2.key() == it->pin) ? btn_pal_checked : btn_pal);
        }
    }

    QString test = QString("%1").arg(it->chip);

    for (int i=0; i <ui->tabChips->count(); ++i) {
        if ( ui->tabChips->tabText(i).indexOf(test) > 0) {
            ui->tabChips->setCurrentIndex(i);
        }
    }
}

void VisualConfig::__resetState(int xy_id, int pin_id)
{

    qDebug()<<"xy_id="<<xy_id;

    for (int i = 0; i < xy.count(); ++i) {
        ((QPushButton*)xy[i])->setChecked(i == xy_id);
        ((QPushButton*)xy[i])->setPalette(i == xy_id?btn_pal_checked:btn_pal);
    }
    for (QMap<int, QMap<int, QPushButton*>*>::iterator it = chip_view.begin(); it!=chip_view.end(); ++it) {
       for (QMap<int, QPushButton*>::iterator it2 = it.value()->begin(); it2 != it.value()->end();++it2) {
           it2.value()->setChecked(  ((pin_id/100) == it.key()) && ((pin_id % 100) == it2.key()) );
            it2.value()->setPalette(((pin_id/100) == it.key()) && ((pin_id % 100) == it2.key())? btn_pal_checked : btn_pal);
        }
    }
}

void VisualConfig::__setExchButtonsState(bool state)
{
    ui->btnConnect->setEnabled(!state);
    ui->btReadSensor->setVisible(state);
    ui->btReadSensor_2->setVisible(state);
    ui->btReadSensor_3->setVisible(state);

    ui->btReadAllSensors->setEnabled(state);
    ui->btReadAllSensors->setFlat(!state);

    ui->btReadFromDev->setEnabled(state);
    ui->btReadFromDev->setFlat(!state);

    ui->btnGo->setEnabled(state);
    ui->btnGo->setFlat(!state);

    ui->btnDown->setEnabled(state);
    ui->btnDown->setFlat(!state);

    ui->btnUp->setEnabled(state);
    ui->btnUp->setFlat(!state);

    ui->btnPipi->setEnabled(state);
    ui->btnPipi->setFlat(!state);

    ui->btnTryAll->setEnabled(state);
    ui->btnTryAll->setFlat(!state);

    ui->btSend->setEnabled(state);
    ui->btSend->setFlat(!state);

    ui->btnGlobalCfg->setEnabled(state);
    ui->btnGlobalCfg->setFlat(!state);

    ui->btnWaterTanks->setEnabled(state);
    ui->btnWaterTanks->setFlat(!state);

}


void VisualConfig::__msg(char *msg)
{
    ui->lbMsg->setText(QString::fromLocal8Bit(msg));
}

void VisualConfig::__msg(const QString &msg)
{
    ui->lbMsg->setText(msg);
}

void VisualConfig::__sendConfigLine(int index)
{
    if (!this->port || !this->port->isOpen()) {
        __msg("Порт недоступен. Обрыв связи?");
        return;
    }
    if (index < 0 ||index >= pots_data->count()) {
        qDebug()<<"cfg ok, setting pots count...";
        state = vcstSendSensorsCount;
        this->port->write(QString("pot set count %1;\r\n").arg(pots_data->count()).toLatin1());
        return;
    }
     PlantData pd = pots_data->at(index);
     int test = pd.chip + pd.pin + pd.X +pd.Y +pd.portion + pd.pgm;
     qDebug()<<"__sendConfigLine #"<<index<<" of "<<pots_data->count()<<" test:"<<test;
     if (test > 0 && pd.X >= 0 && pd.Y >=0) {
//pot set <index>,<flags>,<dev>,<pin>,<x>,<y>,<name>,<airTime>,<state>,<enabled>,<ml>,<pgm>,<param1>[,param2][,param3]...;
         QString line/*("pot set %1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15;\r\n")*/;
         QTextStream stm;
         stm.setString(&line);
         stm<<"pot set "<<index<<","<<
                  //pd.flags<<","<<
                  pd.chip<<","<<
                  pd.pin<<","<<
                  pd.X<<","<<
                  pd.Y<<","<<
                  pd.name<<","<<
                  2/*airtime*/<<","<<
                  0<<","<<
                  pd.en<<","<<
                  pd.portion<<","<<
                  pd.pgm<<",";
         if (pd.pgm == 1) {
                  stm<<pd.max<<","<<
                  pd.daymax;
         } else if (pd.pgm == 2) {
             stm<<pd.min<<","<<pd.max<<","<<pd.daymax;
         }
         stm<<";\r\n";
         qDebug()<<"cfg line "<<line;
         port->write(line.toLatin1());
     } else {
        //pots_data->remove(index);
        __sendConfigLine(index);
     }
}


void VisualConfig::on_btnApply_clicked()
{
    int x = ui->spinX->value(),
        y = ui->spinY->value();
    if (this->pots_data == NULL) {
        pots_data = new QVector<PlantData>;
    }
    if (!__isXYused(x, y)) {
        qDebug()<<"new item";
        PlantData item;
        //memset(&item, 0, sizeof(PlantData));
        item.flags = 0;
        item.index = pots_data->count();
        item.name = ui->edName->text();
        item.X = ui->spinX->value();
        item.Y = ui->spinY->value();
        item.chip = ui->edChip->text().toInt();
        item.pin = ui->edPin->text().toInt();
        item.en = ui->chkEn->isChecked()?1:0;
        item.daymax = ui->edDayMax->text().toInt();
        item.portion = ui->edML->text().toInt();
        item.pgm = ui->rdPgm1->isChecked()?1:2;
        if (item.pgm == 1) {
            item.max = ui->edPgm1Value->text().toInt();
        } else if (item.pgm == 2) {
            item.min = ui->edPgm2MinValue->text().toInt();
            item.max = ui->edPgm2MaxValue->text().toInt();
        }
        pots_data->append(item);
        int index = item.X*szy + item.Y;
        if (index < xy.count()) {
            ((QPushButton*)xy[index])->setText(QString("*"));
        }
        if (chip_view.find(item.chip)!= chip_view.end()) {
            //QMap<int, QMap<int, QLabel*>*> chip_view;
            chip_view.find(item.chip).value()->find(item.pin).value()->setText(QString("%1 %2").arg(item.pin, 2, 10, QChar(' ')).arg(item.name));
        }
        qDebug()<<"chip view set";
    } else {
        qDebug()<<"update item";
        int i = 0;
        for (QVector<PlantData>::iterator it = pots_data->begin(); it != pots_data->end(); ++it,++i) {
            if (it->X == x && it->Y == y) {
                qDebug()<<"index: "<<i;
                it->name = ui->edName->text();
                it->chip = ui->edChip->text().toInt();
                it->pin = ui->edPin->text().toInt();
                it->en = ui->chkEn->isChecked() ? 1 : 0;
                it->daymax = ui->edDayMax->text().toInt();
                it->portion = ui->edML->text().toInt();
                it->pgm = ui->rdPgm1->isChecked()?1:2;
                if (it->pgm == 1) {
                    it->max = ui->edPgm1Value->text().toInt();
                } else if (it->pgm == 2) {
                    it->min = ui->edPgm2MinValue->text().toInt();
                    it->max = ui->edPgm2MaxValue->text().toInt();
                }
                if (chip_view.find(it->chip)!= chip_view.end()) {
                    if (chip_view.find(it->chip).value()->find(it->pin)!= chip_view.find(it->chip).value()->end()) {
                        QPushButton*btn = chip_view.find(it->chip).value()->find(it->pin).value();
                        btn->setText(QString("%1 %2").arg(it->pin, 2, 10, QChar(' ')).arg(it->name));
                        btn->setPalette(btn_pal_checked);
                    }
                }
                break;
            }
        }
    }
}

void VisualConfig::on_rdPgm1_toggled(bool checked)
{
    ui->grpbxPgm1->setVisible(checked);
    ui->grpbxPgm2->setVisible(!checked);

}

void VisualConfig::on_rdPgm2_toggled(bool checked)
{
    ui->grpbxPgm1->setVisible(!checked);
    ui->grpbxPgm2->setVisible(checked);
}

void VisualConfig::on_btnPgm1Apply_clicked()
{
    int chip = ui->edChip->text().toInt(),
           pin = ui->edPin->text().toInt();
    for (QVector<PlantData>::iterator it = pots_data->begin(); it != pots_data->end(); ++it) {
        if (it->chip == chip && it->pin == pin) {
            it->min = ui->edPgm1Value->text().toInt();
            it->max = ui->edPgm1Value->text().toInt();
            return;
        }
    }
}

void VisualConfig::on_btnPgm2ApplyMin_clicked()
{
    int chip = ui->edChip->text().toInt(),
           pin = ui->edPin->text().toInt();
    for (QVector<PlantData>::iterator it = pots_data->begin(); it != pots_data->end(); ++it) {
        if (it->chip == chip && it->pin == pin) {
            it->min = ui->edPgm2MinValue->text().toInt();
            return;
        }
    }
}

void VisualConfig::on_btnPgm2ApplyMax_clicked()
{
    int chip = ui->edChip->text().toInt(),
           pin = ui->edPin->text().toInt();
    for (QVector<PlantData>::iterator it = pots_data->begin(); it != pots_data->end(); ++it) {
        if (it->chip == chip && it->pin == pin) {
            it->max = ui->edPgm2MaxValue->text().toInt();
            return;
        }
    }
}

void VisualConfig::on_btnPgm1Revert_clicked()
{
    int chip = ui->edChip->text().toInt(),
           pin = ui->edPin->text().toInt();
    for (QVector<PlantData>::iterator it = pots_data->begin(); it != pots_data->end(); ++it) {
        if (it->chip == chip && it->pin == pin) {
            ui->edPgm1Value->setText(QString("%1").arg(it->max));
            return;
        }
    }
}

void VisualConfig::on_btnPgm2RevertMin_clicked()
{
    int chip = ui->edChip->text().toInt(),
           pin = ui->edPin->text().toInt();
    for (QVector<PlantData>::iterator it = pots_data->begin(); it != pots_data->end(); ++it) {
        if (it->chip == chip && it->pin == pin) {
            ui->edPgm2MinValue->setText(QString("%1").arg(it->min));
            return;
        }
    }
}

void VisualConfig::on_btnPgm2RevertMax_clicked()
{
    int chip = ui->edChip->text().toInt(),
           pin = ui->edPin->text().toInt();
    for (QVector<PlantData>::iterator it = pots_data->begin(); it != pots_data->end(); ++it) {
        if (it->chip == chip && it->pin == pin) {
            ui->edPgm2MaxValue->setText(QString("%1").arg(it->max));
            return;
        }
    }
}

void VisualConfig::on_btnUp_clicked()
{
    if (this->port && this->port->isOpen()) {
        port->write("U;\r\n");
    }
}

void VisualConfig::on_btnDown_clicked()
{
    if (this->port && this->port->isOpen()) {
        port->write("D;\r\n");
    }
}

void VisualConfig::on_btnGo_clicked()
{
    if (!port || !port->isOpen()) return;
    QTextStream stm;
    QString cmd;
    stm.setString(&cmd);
    stm<<"G";
    if (ui->chkRestart->isChecked()) stm<<"!";
    stm<<ui->spinX->value()<<","<<ui->spinY->value()<<";\r\n";
    state = vcstNone;
    qDebug()<<cmd;
    port->write(cmd.toLocal8Bit());
}

void VisualConfig::on_btnPipi_clicked()
{
    if (!port || !port->isOpen()) return;
    QString cmd("pipi %1,%2,%3;\r\n");
    cmd = cmd.arg(ui->spinX->value()).arg(ui->spinY->value()).arg(ui->spinML->value());
    state = vcstNone;
    port->write(cmd.toLocal8Bit());
}

void VisualConfig::on_btnTryAll_clicked()
{
    if (!port || !port->isOpen()) return;
    state = vcstNone;
    port->write("try all;\r\n");
}

void VisualConfig::on_btOpen_clicked()
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

    if (pots_data) {
        pots_data->clear();
    } else {
        pots_data = new QVector<PlantData>;
    }

    while ( ++i < n) {
        PlantData pd;
        QString key, val;
        key = key.sprintf("pot_%d/name", i);
        val = settings.value(key, "").toString();
        pd.name = val;

        key = key.sprintf("pot_%d/flags", i);
        val = settings.value(key, "").toString();

        pd.flags = val.toInt();

        key = key.sprintf("pot_%d/pgm", i);
        val = settings.value(key, "").toString();
        pd.pgm = val.toInt();

        key = key.sprintf("pot_%d/chip", i);
        val = settings.value(key, "").toString();
        pd.chip = val.toInt();

        key = key.sprintf("pot_%d/pin", i);
        val = settings.value(key, "").toString();
        pd.pin = val.toInt();

        key = key.sprintf("pot_%d/X", i);
        val = settings.value(key, "").toString();
        pd.X = val.toInt();

        key = key.sprintf("pot_%d/Y", i);
        val = settings.value(key, "").toString();
        pd.Y = val.toInt();

        key = key.sprintf("pot_%d/dose", i);
        val = settings.value(key, "").toString();
        pd.portion = val.toInt();

        key = key.sprintf("pot_%d/min", i);
        val = settings.value(key, "").toString();
        pd.min = val.toInt();

        key = key.sprintf("pot_%d/max", i);
        val = settings.value(key, "").toString();
        pd.max = val.toInt();
        key = key.sprintf("pot_%d/daymax", i);
        val = settings.value(key, "").toString();
        pd.daymax = val.toInt();

        key = key.sprintf("pot_%d/isOn", i);
        val = settings.value(key, "").toString();
        pd.en = val.toInt();
        pd.index = i;
        pots_data->push_back(pd);
    }//while
    this->setPotsData(pots_data);
}

void VisualConfig::on_btSave_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.ini)"));
    QSettings settings(fileName/*QString("configs/sensors.ini")*/, QSettings::IniFormat);
    int i = 0;
    qDebug()<<fileName;
    for(QVector<PlantData>::iterator it=pots_data->begin(); it!=pots_data->end(); ++i, ++it) {
        QString key;
        qDebug()<<"row "<<i;
        key = key.sprintf("pot_%d/flags", i);
        settings.setValue(key,  it->flags);
        key = key.sprintf("pot_%d/name", i);
        settings.setValue(key,  it->name);
        key = key.sprintf("pot_%d/chip", i);
        settings.setValue(key,  it->chip);
        key = key.sprintf("pot_%d/pin", i);
        settings.setValue(key,  it->pin);
        key = key.sprintf("pot_%d/X", i);
        settings.setValue(key,  it->X);
        key = key.sprintf("pot_%d/Y", i);
        settings.setValue(key,  it->Y);
        key = key.sprintf("pot_%d/dose", i);
        settings.setValue(key,  it->portion);
        key = key.sprintf("pot_%d/pgm", i);
        settings.setValue(key,  it->pgm);
        key = key.sprintf("pot_%d/min", i);
        settings.setValue(key,  it->min);
        key = key.sprintf("pot_%d/max", i);
        settings.setValue(key,  it->max);
        key = key.sprintf("pot_%d/isOn", i);
        settings.setValue(key,  it->en);
        key = key.sprintf("pot_%d/daymax", i);
        settings.setValue(key,  it->daymax);

    }
    settings.setValue("global/count", pots_data->count());

    settings.setValue("global/sz_x", ui->spinX->maximum());
    settings.setValue("global/sz_y", ui->spinY->maximum());
    //ui->statusBar->showMessage(QString::fromLocal8Bit("Конфигурация сохранена локально."));

}

void VisualConfig::on_btReadFromDev_clicked()
{
    if (!(port && port->isOpen())) {
        return;
    }
    QMutexLocker ml(mtx);
    state = vcstReadConfig;
    port->flush();
    buf.clear();
    last_index = -1;
    pots_total = -1;
    if (this->pots_data) {
        this->pots_data->clear();
    }
    port->write("pot get count;\r\n");
}

void VisualConfig::on_btReadSensor_clicked()
{
    QString cmd;
    QTextStream stm;
    stm.setString(&cmd);
    stm<<"iic "<< ui->edChip->text()<<","<<ui->edPin->text()<<";\r\n";
    port->write(cmd.toLocal8Bit());
    pinValueEdit = ui->edPgm1Value;
    state = vcstReadPin;
}

void VisualConfig::on_btReadSensor_2_clicked()
{
    QString cmd;
    QTextStream stm;
    stm.setString(&cmd);
    stm<<"iic "<< ui->edChip->text()<<","<<ui->edPin->text()<<";\r\n";
    port->write(cmd.toLocal8Bit());
    pinValueEdit = ui->edPgm2MinValue;
    state = vcstReadPin;
}

void VisualConfig::on_btReadSensor_3_clicked()
{
    QString cmd;
    QTextStream stm;
    stm.setString(&cmd);
    stm<<"iic "<< ui->edChip->text()<<","<<ui->edPin->text()<<";\r\n";
    port->write(cmd.toLocal8Bit());
    pinValueEdit = ui->edPgm2MaxValue;
    state = vcstReadPin;
}

void VisualConfig::on_btReadAllSensors_clicked()
{
    if (!(port && port->isOpen())) {
        return;
    }
    QMutexLocker ml(mtx);
    state = vcstReadCurStateStep1;
    port->flush();
    buf.clear();
    last_index = -1;
    port->write("sdump;\r\n");
}

void VisualConfig::on_btnConnect_clicked()
{
    ConnectDlg dlg(this);
    if (dlg.exec()) {
        __msg("Порт открыт, соединение установлено");
        qDebug()<<"port opened "<<dlg.getDeviceName()<<" "<<dlg.getAddress();
        port = dlg.getPort();
        qDebug()<<port;
        ui->btReadFromDev->setEnabled(true);
        connect(port, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
        __setExchButtonsState(true);
        if (dlg.isAutoloadConfig()) {
            state = vcstReadSize;
            port->write("WSZ;\r\n");
            //on_btReadFromDev_clicked();
        }
    } else {
        __msg("Нет соединения, работаем локально");
        __setExchButtonsState(false);
        qDebug()<<"cancelled";
    }
}

void VisualConfig::on_btSend_clicked()
{
    last_index = 0;
    state = vcstSendConfig;
    QMutexLocker ml(mtx);
    buf.clear();
    bool removed = false;
    do {
        removed = false;
        for (QVector<PlantData>::iterator it=pots_data->begin(); it!=pots_data->end(); ++it) {
            PlantData pd = *it;
            qDebug()<<pd.X<<","<<pd.Y;
            if (pd.X < 0 && pd.Y < 0 || pd.chip <=0 || pd.pin < 0) {
                qDebug()<<"remove";
                pots_data->erase(it);
                removed = true;
                break;
            }
        }
    } while (removed);
    __sendConfigLine(last_index);
}

void VisualConfig::on_btnGlobalCfg_clicked()
{
    GlobalStateDialog dlg;
    disconnect(port, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    dlg.setPort(port);
    dlg.exec();
    connect(port, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
}

void VisualConfig::on_btnWaterTanks_clicked()
{
    WaterStorages dlg;
    disconnect(port, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    dlg.setPort(port);
    dlg.exec();
    connect(port, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
}

void VisualConfig::on_edRawCmd_returnPressed()
{
    if(port == NULL || !port->isOpen()) {
        qDebug()<<"port is closed";
        return;
    }
    QMutexLocker ml(mtx);
    buf.clear();
    state = vcstReadRaw;
    port->write(ui->edRawCmd->text().append("\r\n").toLocal8Bit());
}

void VisualConfig::on_btnLogClear_clicked()
{
    ui->txtLog->clear();
}
