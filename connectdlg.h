#ifndef CONNECTDLG_H
#define CONNECTDLG_H

#include <QDialog>
#include <com/qextserialport.h>
#include<QProcess>

namespace Ui {
class ConnectDlg;
}

class ConnectDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConnectDlg(QWidget *parent = 0);
    ~ConnectDlg();
    QextSerialPort*getPort();
    QString getDeviceName();
    QString getAddress();
    bool isAutoloadConfig();
private slots:
    void on_btnObserve_clicked();
    void onScanFinished (int exitCode, QProcess::ExitStatus exitStatus);
    void on_btnConnect_clicked();

private:
    Ui::ConnectDlg *ui;
    QextSerialPort*port;
    QString name, addr;
    QProcess* scanner;
    int auto_connect;
    QString auto_cnx_addr;
    bool __connect();
    bool auto_load_cfg;
protected:
    virtual void showEvent( QShowEvent* event);
};

#endif // CONNECTDLG_H
