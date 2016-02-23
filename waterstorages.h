#ifndef WATERSTORAGES_H
#define WATERSTORAGES_H

#include <QDialog>
#include "com/qextserialport.h"

typedef enum {
    wstNone,
    wstReadData,
    wstSendData,
    wstCmdSent
} wstState;

namespace Ui {
    class WaterStorages;
}

class WaterStorages : public QDialog
{
    Q_OBJECT
    
public:
    explicit WaterStorages(QWidget *parent = 0);
    ~WaterStorages();
    void setPort(QextSerialPort*_port);
private slots:
    void on_btnReadData_clicked();
    void onDataAvailable();
    void on_btnMarkRefilled_clicked();

    void on_btnSendData_clicked();

    void on_btnAddStorage_clicked();

    void on_btnDeleteStorage_clicked();

private:
    Ui::WaterStorages *ui;
    QextSerialPort*port;
    wstState state;
    QMutex*mtx;
    QString buf;
};

#endif // WATERSTORAGES_H
