#ifndef GLOBALSTATEDIALOG_H
#define GLOBALSTATEDIALOG_H

#include <QDialog>
#include "com/qextserialport.h"

namespace Ui {
class GlobalStateDialog;
}

typedef enum{
    gstNone,
    gstReadState,
    gstSendCommand,
    gstReadCfg,
    gstReadTime
} enumGSDState;

class GlobalStateDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GlobalStateDialog(QWidget *parent = 0);
    ~GlobalStateDialog();
    void setPort(QextSerialPort*port);
private:
    Ui::GlobalStateDialog *ui;
    QextSerialPort*port;
    enumGSDState state;
    QString buf;
    QMutex*mtx;
    unsigned int saved_flags;
    void updateData();
private slots:
    void onDataAvailable();
    void on_btRefresh_clicked();
    void on_btTestPos_clicked();
    void on_btnServoTest_clicked();
    void on_btnGetTime_clicked();
    void on_btnApplyAll_clicked();
    void on_btnSetTime_clicked();
};

#endif // GLOBALSTATEDIALOG_H
