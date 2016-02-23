#ifndef VISUALCONFIG_H
#define VISUALCONFIG_H

#include <QMainWindow>
#include "com/qextserialport.h"
#include <QPushButton>
#include <QVector>
#include <QLabel>
#include <QMap>
#include <QGridLayout>
#include <QSignalMapper>
#include "data_structs.h"
#include<QPalette>
#include <QLineEdit>

namespace Ui {
class VisualConfig;
}
typedef enum vcStates{
    vcstNone,
    vcstReadSize,
    vcstReadConfig,
    vcstGetW2Dsizes,
    vcstReadPin,
    vcstReadCurStateStep1,
    vcstReadCurStateStep2,
    vcstSendConfig,
    vcstSendSensorsCount,
    vcstReadRaw
} vcStates;

class VisualConfig : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit VisualConfig(QWidget *parent = 0);
    ~VisualConfig();
    void setPort(QextSerialPort*port);
    void setWaterDoserSizes(int _x, int _y);
    void setPotsData(QVector<PlantData>*data);
public slots:
    void xy_clicked(int id);
    void pinOfChip_clicked(int id);
private slots:
    void dataAvailable();
    void on_btnApply_clicked();

    void on_rdPgm1_toggled(bool checked);

    void on_rdPgm2_toggled(bool checked);

    void on_btnPgm1Apply_clicked();

    void on_btnPgm2ApplyMin_clicked();

    void on_btnPgm2ApplyMax_clicked();

    void on_btnPgm1Revert_clicked();

    void on_btnPgm2RevertMin_clicked();

    void on_btnPgm2RevertMax_clicked();

    void on_btnUp_clicked();

    void on_btnDown_clicked();

    void on_btnGo_clicked();

    void on_btnPipi_clicked();

    void on_btnTryAll_clicked();

    void on_btOpen_clicked();

    void on_btSave_clicked();

    void on_btReadFromDev_clicked();

    void on_btReadSensor_clicked();

    void on_btReadSensor_2_clicked();

    void on_btReadSensor_3_clicked();

    void on_btReadAllSensors_clicked();

    void on_btnConnect_clicked();

    void on_btSend_clicked();

    void on_btnGlobalCfg_clicked();

    void on_btnWaterTanks_clicked();

    void on_edRawCmd_returnPressed();

    void on_btnLogClear_clicked();

private:
    int szx, szy;
    bool b_new;
    QVector<PlantData>* pots_data;
    Ui::VisualConfig *ui;
    QextSerialPort*port;
    vcStates state;
    QVector<QWidget*> xy;
    QMap<int, QMap<int, QPushButton*>*> chip_view;
    QMap<int, QMap<int, QLabel*>*> sensor_values;
    QGridLayout*lay;
    QString buf;
    QMutex*mtx;
    QLineEdit *pinValueEdit;
    QSignalMapper*sigmap, *chip_sigmap;
    bool __isXYused(int x,int y);
    int cur_pot_index, last_index, pots_total;
    QPalette btn_pal, btn_pal_checked, chip_addr_label_palette;
    void __setCurrentPot(QVector<PlantData>::const_iterator& it);
    void __resetState(int xy_id=-1, int pin_id=-1);
    void __setExchButtonsState(bool state);
    void __msg(char*msg);
    void __msg(const QString&msg);
    void __sendConfigLine(int index);
};

#endif // VISUALCONFIG_H
