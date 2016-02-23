#include "connectdlg.h"
#include "ui_connectdlg.h"
#include<QSettings>
#include<QDebug>

ConnectDlg::ConnectDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDlg), auto_cnx_addr("")
{
    qDebug()<<"ConnectDlg";
    this->scanner = 0;
    port = NULL;
    ui->setupUi(this);
}

ConnectDlg::~ConnectDlg()
{
    if(result() == Accepted) {
        QSettings settings(QString("configs/connection.ini"), QSettings::IniFormat);
        settings.setValue("main/autoscan", ui->chkScanOnStart->isChecked());

        settings.setValue("main/autoconnect", ui->chkAutoConnect->isChecked());
        settings.setValue("main/autoload_cfg", ui->chkAutoLoadCfg->isChecked());
        if (auto_connect) {
            settings.setValue("main/autoconnect_address", QVariant(addr));
        }
    }

    delete ui;    
}

QextSerialPort *ConnectDlg::getPort()
{
    return port;
}

QString ConnectDlg::getDeviceName()
{
    return name;
}

QString ConnectDlg::getAddress()
{
    return addr;
}

bool ConnectDlg::isAutoloadConfig()
{
    return ui->chkAutoLoadCfg->isChecked();
}

void ConnectDlg::on_btnObserve_clicked()
{
    ui->lbState->setText(QString::fromLocal8Bit("Поиск устройств..."));
    this->scanner = new QProcess(this);
    QString program("/usr/bin/hcitool");
    QStringList arguments;
    arguments << "scan";
    QObject::connect(this->scanner, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onScanFinished(int, QProcess::ExitStatus)));
    this->scanner->setProcessChannelMode(QProcess::MergedChannels);
    this->scanner->start(program, arguments);
}

void ConnectDlg::onScanFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->lbState->setText(QString::fromLocal8Bit("Поиск завершён"));
    QString output(this->scanner->readAllStandardOutput());

    delete this->scanner;
    this->scanner = NULL;

    QStringList devs = output.split(QChar('\n'));
    if (devs.count() < 2 ) {
        ui->lbState->setText(QString::fromLocal8Bit("Поиск устройств не дал результатов"));
        return;
    }
    QRegExp re("\\s*([0-9a-z\\:]+)\\s*(\\w+)", Qt::CaseInsensitive);
    this->ui->cmbDevice->clear();
    for (int i = 1; i < devs.count(); ++i) {
        QString s = devs.at(i);
        if (re.indexIn(s) > -1) {
            this->ui->cmbDevice->addItem(re.cap(2).append(" [").append(re.cap(1)).append("]"), re.cap(1));
            if (!auto_cnx_addr.isEmpty() && auto_cnx_addr == re.cap(1)) {
                addr = re.cap(1);
                name = re.cap(2);
                if (__connect()) {
                  this->accept();
                } else {
                  this->reject();
                }

            }
        }
    }//for i
    if (this->ui->cmbDevice->count() > 0) {
        this->ui->btnConnect->setEnabled(true);
        ui->cmbDevice->setEnabled(true);
    }
    ui->lbState->setText(QString::fromLocal8Bit("Поиск завершён. Выберите устройство для связи."));
}

void ConnectDlg::on_btnConnect_clicked()
{

    addr = ui->cmbDevice->itemData((ui->cmbDevice->currentIndex())).toString();

    name = ui->cmbDevice->itemText(ui->cmbDevice->currentIndex());
    if (__connect()) {
        this->accept();
    } else {
        this->reject();
    }
}

bool ConnectDlg::__connect()
{

    ui->lbState->setText(QString::fromLocal8Bit("Соединяемся с %1").arg(this->addr));
    if (this->port) {
        port->disconnect();
        port->close();
        delete port;
        port = NULL;
    }

    QString cmd = QString("rfcomm unbind all;rfcomm bind rfcomm0 %1;").arg(addr);

    system(cmd.toLocal8Bit());

    sleep(1);
    PortSettings ps;
    ps.BaudRate = BAUD38400;
    ps.DataBits = DATA_8;
    ps.Parity = PAR_NONE;
    ps.StopBits = STOP_1;
    ps.FlowControl = FLOW_OFF;
    ps.Timeout_Millisec = 5;

    this->port = new QextSerialPort("/dev/rfcomm0", ps, QextSerialPort::EventDriven);
    if (!this->port->open(QIODevice::ReadWrite|QIODevice::Text)) {
        ui->lbState->setText(this->port->errorString());
        delete this->port;
        this->port= NULL;

        return false;
    } else {
        return true;
    }
}

void ConnectDlg::showEvent(QShowEvent *event)
{
    qDebug()<<"ConnectDlg::showEvent";
    QDialog::showEvent(event);
    QSettings settings(QString("configs/connection.ini"), QSettings::IniFormat);
    bool auto_scan = settings.value("main/autoscan", QVariant(false)).toBool();
    qDebug()<<"auto_scan="<<auto_scan;

    auto_connect = settings.value("main/autoconnect", QVariant(false)).toBool();
    ui->chkAutoConnect->setChecked(auto_connect);
    if (auto_connect) {
        auto_cnx_addr = settings.value("main/autoconnect_address", "").toString();
        qDebug()<<"auto_cnx_addr="<<auto_cnx_addr;
    }

    if (auto_scan) {
        ui->chkScanOnStart->setChecked(auto_scan);
        this->on_btnObserve_clicked();
    }
    auto_load_cfg = settings.value("main/autoload_cfg", QVariant(false)).toBool();
    ui->chkAutoLoadCfg->setChecked(auto_load_cfg);
}
