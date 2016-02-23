/********************************************************************************
** Form generated from reading UI file 'makeconfig.ui'
**
** Created: Wed Oct 7 21:12:39 2015
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAKECONFIG_H
#define UI_MAKECONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MakeConfig
{
public:
    QAction *mnuExit;
    QAction *mnuSetup;
    QAction *action_2;
    QAction *mnuLoadFromDev;
    QAction *mnuAddRow;
    QAction *mnuTestWatering;
    QAction *actWST;
    QAction *mnuVisualCfg;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *btObserve;
    QComboBox *cmbDevice;
    QPushButton *btnConnect;
    QPushButton *btDetect;
    QPushButton *btDetect2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btApplyDetected;
    QPushButton *btSave;
    QPushButton *btOpen;
    QPushButton *btnTestWater;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnSort;
    QPushButton *btSend;
    QPushButton *btnGetVals;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label;
    QSpinBox *spnRows;
    QLabel *label_2;
    QComboBox *cbChipType;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QSpinBox *spinX;
    QLabel *label_3;
    QSpinBox *spinY;
    QCheckBox *chkRestart;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *btnGo;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *btnUp;
    QPushButton *btnDown;
    QSpacerItem *horizontalSpacer_6;
    QSpinBox *spinML;
    QLabel *label_5;
    QPushButton *btnPipi;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *btnTryAll;
    QSpacerItem *horizontalSpacer_9;
    QLineEdit *edCmd;
    QPushButton *btnCmd;
    QTableWidget *tblConfig;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *mnuTable;
    QMenu *menu_2;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MakeConfig)
    {
        if (MakeConfig->objectName().isEmpty())
            MakeConfig->setObjectName(QString::fromUtf8("MakeConfig"));
        MakeConfig->resize(1004, 592);
        mnuExit = new QAction(MakeConfig);
        mnuExit->setObjectName(QString::fromUtf8("mnuExit"));
        mnuSetup = new QAction(MakeConfig);
        mnuSetup->setObjectName(QString::fromUtf8("mnuSetup"));
        action_2 = new QAction(MakeConfig);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        mnuLoadFromDev = new QAction(MakeConfig);
        mnuLoadFromDev->setObjectName(QString::fromUtf8("mnuLoadFromDev"));
        mnuAddRow = new QAction(MakeConfig);
        mnuAddRow->setObjectName(QString::fromUtf8("mnuAddRow"));
        mnuAddRow->setCheckable(true);
        mnuTestWatering = new QAction(MakeConfig);
        mnuTestWatering->setObjectName(QString::fromUtf8("mnuTestWatering"));
        actWST = new QAction(MakeConfig);
        actWST->setObjectName(QString::fromUtf8("actWST"));
        mnuVisualCfg = new QAction(MakeConfig);
        mnuVisualCfg->setObjectName(QString::fromUtf8("mnuVisualCfg"));
        centralWidget = new QWidget(MakeConfig);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btObserve = new QPushButton(centralWidget);
        btObserve->setObjectName(QString::fromUtf8("btObserve"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/res/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        btObserve->setIcon(icon);

        horizontalLayout->addWidget(btObserve);

        cmbDevice = new QComboBox(centralWidget);
        cmbDevice->setObjectName(QString::fromUtf8("cmbDevice"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cmbDevice->sizePolicy().hasHeightForWidth());
        cmbDevice->setSizePolicy(sizePolicy);
        cmbDevice->setMinimumSize(QSize(180, 0));
        cmbDevice->setBaseSize(QSize(240, 0));

        horizontalLayout->addWidget(cmbDevice);

        btnConnect = new QPushButton(centralWidget);
        btnConnect->setObjectName(QString::fromUtf8("btnConnect"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/res/connect.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnConnect->setIcon(icon1);

        horizontalLayout->addWidget(btnConnect);

        btDetect = new QPushButton(centralWidget);
        btDetect->setObjectName(QString::fromUtf8("btDetect"));
        btDetect->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btDetect->sizePolicy().hasHeightForWidth());
        btDetect->setSizePolicy(sizePolicy1);
        btDetect->setMaximumSize(QSize(0, 0));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/res/find.png"), QSize(), QIcon::Normal, QIcon::Off);
        btDetect->setIcon(icon2);

        horizontalLayout->addWidget(btDetect);

        btDetect2 = new QPushButton(centralWidget);
        btDetect2->setObjectName(QString::fromUtf8("btDetect2"));
        btDetect2->setMaximumSize(QSize(0, 0));
        btDetect2->setIcon(icon2);

        horizontalLayout->addWidget(btDetect2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        btApplyDetected = new QPushButton(centralWidget);
        btApplyDetected->setObjectName(QString::fromUtf8("btApplyDetected"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/res/apply.png"), QSize(), QIcon::Normal, QIcon::Off);
        btApplyDetected->setIcon(icon3);

        horizontalLayout->addWidget(btApplyDetected);

        btSave = new QPushButton(centralWidget);
        btSave->setObjectName(QString::fromUtf8("btSave"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/res/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        btSave->setIcon(icon4);

        horizontalLayout->addWidget(btSave);

        btOpen = new QPushButton(centralWidget);
        btOpen->setObjectName(QString::fromUtf8("btOpen"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/res/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        btOpen->setIcon(icon5);

        horizontalLayout->addWidget(btOpen);

        btnTestWater = new QPushButton(centralWidget);
        btnTestWater->setObjectName(QString::fromUtf8("btnTestWater"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8("water_hose.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnTestWater->setIcon(icon6);

        horizontalLayout->addWidget(btnTestWater);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btnSort = new QPushButton(centralWidget);
        btnSort->setObjectName(QString::fromUtf8("btnSort"));

        horizontalLayout->addWidget(btnSort);

        btSend = new QPushButton(centralWidget);
        btSend->setObjectName(QString::fromUtf8("btSend"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/res/transmit.png"), QSize(), QIcon::Normal, QIcon::Off);
        btSend->setIcon(icon7);

        horizontalLayout->addWidget(btSend);

        btnGetVals = new QPushButton(centralWidget);
        btnGetVals->setObjectName(QString::fromUtf8("btnGetVals"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/res/download.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnGetVals->setIcon(icon8);

        horizontalLayout->addWidget(btnGetVals);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        spnRows = new QSpinBox(centralWidget);
        spnRows->setObjectName(QString::fromUtf8("spnRows"));
        spnRows->setValue(20);

        horizontalLayout->addWidget(spnRows);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        cbChipType = new QComboBox(centralWidget);
        cbChipType->setObjectName(QString::fromUtf8("cbChipType"));

        horizontalLayout->addWidget(cbChipType);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout_3->setContentsMargins(0, 5, -1, -1);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);
        label_4->setMinimumSize(QSize(20, 0));

        horizontalLayout_3->addWidget(label_4);

        spinX = new QSpinBox(centralWidget);
        spinX->setObjectName(QString::fromUtf8("spinX"));
        sizePolicy2.setHeightForWidth(spinX->sizePolicy().hasHeightForWidth());
        spinX->setSizePolicy(sizePolicy2);
        spinX->setMinimumSize(QSize(30, 0));

        horizontalLayout_3->addWidget(spinX);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy2.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy2);
        label_3->setMinimumSize(QSize(20, 0));

        horizontalLayout_3->addWidget(label_3);

        spinY = new QSpinBox(centralWidget);
        spinY->setObjectName(QString::fromUtf8("spinY"));
        sizePolicy2.setHeightForWidth(spinY->sizePolicy().hasHeightForWidth());
        spinY->setSizePolicy(sizePolicy2);
        spinY->setMinimumSize(QSize(30, 0));

        horizontalLayout_3->addWidget(spinY);

        chkRestart = new QCheckBox(centralWidget);
        chkRestart->setObjectName(QString::fromUtf8("chkRestart"));

        horizontalLayout_3->addWidget(chkRestart);

        horizontalSpacer_7 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);

        btnGo = new QPushButton(centralWidget);
        btnGo->setObjectName(QString::fromUtf8("btnGo"));
        sizePolicy1.setHeightForWidth(btnGo->sizePolicy().hasHeightForWidth());
        btnGo->setSizePolicy(sizePolicy1);
        btnGo->setMinimumSize(QSize(50, 0));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icons/res/moveto.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnGo->setIcon(icon9);
        btnGo->setCheckable(false);
        btnGo->setChecked(false);
        btnGo->setFlat(false);

        horizontalLayout_3->addWidget(btnGo);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_8);

        btnUp = new QPushButton(centralWidget);
        btnUp->setObjectName(QString::fromUtf8("btnUp"));
        sizePolicy1.setHeightForWidth(btnUp->sizePolicy().hasHeightForWidth());
        btnUp->setSizePolicy(sizePolicy1);
        btnUp->setMinimumSize(QSize(50, 0));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icons/res/lift_up.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnUp->setIcon(icon10);
        btnUp->setIconSize(QSize(16, 16));

        horizontalLayout_3->addWidget(btnUp);

        btnDown = new QPushButton(centralWidget);
        btnDown->setObjectName(QString::fromUtf8("btnDown"));
        sizePolicy1.setHeightForWidth(btnDown->sizePolicy().hasHeightForWidth());
        btnDown->setSizePolicy(sizePolicy1);
        btnDown->setMinimumSize(QSize(50, 0));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/icons/res/lift_down.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDown->setIcon(icon11);

        horizontalLayout_3->addWidget(btnDown);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        spinML = new QSpinBox(centralWidget);
        spinML->setObjectName(QString::fromUtf8("spinML"));
        spinML->setMinimum(5);
        spinML->setMaximum(100);
        spinML->setSingleStep(5);
        spinML->setValue(10);

        horizontalLayout_3->addWidget(spinML);

        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_3->addWidget(label_5);

        btnPipi = new QPushButton(centralWidget);
        btnPipi->setObjectName(QString::fromUtf8("btnPipi"));
        sizePolicy1.setHeightForWidth(btnPipi->sizePolicy().hasHeightForWidth());
        btnPipi->setSizePolicy(sizePolicy1);
        btnPipi->setMinimumSize(QSize(50, 0));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/icons/res/singledrop.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPipi->setIcon(icon12);

        horizontalLayout_3->addWidget(btnPipi);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        btnTryAll = new QPushButton(centralWidget);
        btnTryAll->setObjectName(QString::fromUtf8("btnTryAll"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/icons/res/water.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnTryAll->setIcon(icon13);

        horizontalLayout_3->addWidget(btnTryAll);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_9);

        edCmd = new QLineEdit(centralWidget);
        edCmd->setObjectName(QString::fromUtf8("edCmd"));
        sizePolicy.setHeightForWidth(edCmd->sizePolicy().hasHeightForWidth());
        edCmd->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(edCmd);

        btnCmd = new QPushButton(centralWidget);
        btnCmd->setObjectName(QString::fromUtf8("btnCmd"));

        horizontalLayout_3->addWidget(btnCmd);


        verticalLayout->addLayout(horizontalLayout_3);

        tblConfig = new QTableWidget(centralWidget);
        tblConfig->setObjectName(QString::fromUtf8("tblConfig"));

        verticalLayout->addWidget(tblConfig);

        MakeConfig->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MakeConfig);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1004, 21));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        mnuTable = new QMenu(menuBar);
        mnuTable->setObjectName(QString::fromUtf8("mnuTable"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        MakeConfig->setMenuBar(menuBar);
        statusBar = new QStatusBar(MakeConfig);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MakeConfig->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(mnuTable->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addSeparator();
        menu->addAction(mnuExit);
        mnuTable->addAction(mnuAddRow);
        menu_2->addAction(mnuSetup);
        menu_2->addAction(mnuLoadFromDev);
        menu_2->addAction(mnuTestWatering);
        menu_2->addAction(actWST);
        menu_2->addAction(mnuVisualCfg);

        retranslateUi(MakeConfig);

        cbChipType->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MakeConfig);
    } // setupUi

    void retranslateUi(QMainWindow *MakeConfig)
    {
        MakeConfig->setWindowTitle(QApplication::translate("MakeConfig", "Waterer configurator", 0, QApplication::UnicodeUTF8));
        mnuExit->setText(QApplication::translate("MakeConfig", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
        mnuSetup->setText(QApplication::translate("MakeConfig", "\320\223\320\273\320\276\320\261\320\260\320\273\321\214\320\275\321\213\320\265 \320\275\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270...", 0, QApplication::UnicodeUTF8));
        action_2->setText(QApplication::translate("MakeConfig", "-", 0, QApplication::UnicodeUTF8));
        mnuLoadFromDev->setText(QApplication::translate("MakeConfig", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \321\201 \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\260", 0, QApplication::UnicodeUTF8));
        mnuAddRow->setText(QApplication::translate("MakeConfig", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\201\321\202\321\200\320\276\320\272\321\203", 0, QApplication::UnicodeUTF8));
        mnuTestWatering->setText(QApplication::translate("MakeConfig", "\320\242\320\265\321\201\321\202\320\276\320\262\321\213\320\271 \320\277\320\276\320\273\320\270\320\262 \321\202\320\265\320\272\321\203\321\211\320\265\320\271 \321\201\321\202\321\200\320\276\320\272\320\270", 0, QApplication::UnicodeUTF8));
        actWST->setText(QApplication::translate("MakeConfig", "\320\222\320\276\320\264\320\276\321\205\321\200\320\260\320\275\320\270\320\273\320\270\321\211\320\260", 0, QApplication::UnicodeUTF8));
        mnuVisualCfg->setText(QApplication::translate("MakeConfig", "\320\222\320\270\320\267\321\203\320\260\320\273\321\214\320\275\321\213\320\271 \320\272\320\276\320\275\321\204\320\270\320\263\321\203\321\200\320\260\321\202\320\276\321\200", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btObserve->setToolTip(QApplication::translate("MakeConfig", "\320\237\320\276\320\270\321\201\320\272 \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btObserve->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnConnect->setToolTip(QApplication::translate("MakeConfig", "\320\241\320\276\320\265\320\264\320\270\320\275\320\270\321\202\321\214\321\201\321\217", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btnConnect->setText(QString());
#ifndef QT_NO_TOOLTIP
        btDetect->setToolTip(QApplication::translate("MakeConfig", "\320\235\320\260\320\271\321\202\320\270 \321\201\320\265\320\275\321\201\320\276\321\200,\321\210 1", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btDetect->setText(QApplication::translate("MakeConfig", "\321\210\320\260\320\263 I", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btDetect2->setToolTip(QApplication::translate("MakeConfig", "\320\235\320\260\320\271\321\202\320\270 \321\201\320\265\320\275\321\201\320\276\321\200, \321\2102", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btDetect2->setText(QApplication::translate("MakeConfig", "\320\250\320\260\320\263 II", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btApplyDetected->setToolTip(QApplication::translate("MakeConfig", "\320\227\320\260\320\277\320\270\321\201\320\260\321\202\321\214 \321\201\320\265\320\275\321\201\320\276\321\200 \320\262 \321\202\320\265\320\272\321\203\321\211\321\203\321\216 \321\201\321\202\321\200\320\276\320\272\321\203", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btApplyDetected->setText(QString());
#ifndef QT_NO_TOOLTIP
        btSave->setToolTip(QApplication::translate("MakeConfig", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btSave->setText(QString());
#ifndef QT_NO_TOOLTIP
        btOpen->setToolTip(QApplication::translate("MakeConfig", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btOpen->setText(QString());
        btnTestWater->setText(QString());
        btnSort->setText(QApplication::translate("MakeConfig", "Sort", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btSend->setToolTip(QApplication::translate("MakeConfig", "\320\236\321\202\320\277\321\200\320\260\320\262\320\270\321\202\321\214 \320\272\320\276\320\275\321\204\320\270\320\263", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btSend->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnGetVals->setToolTip(QApplication::translate("MakeConfig", "\320\227\320\260\320\277\321\200\320\276\321\201\320\270\321\202\321\214 \321\202\320\265\320\272\321\203\321\211\320\265\320\265 \321\201\320\276\321\201\321\202\320\276\321\217\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btnGetVals->setText(QString());
        label->setText(QApplication::translate("MakeConfig", "\320\232\320\276\320\273-\320\262\320\276 \321\201\321\202\321\200\320\276\320\272", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MakeConfig", "\321\202\320\270\320\277 \321\207\320\270\320\277\320\276\320\262", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MakeConfig", "X", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MakeConfig", "Y", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chkRestart->setToolTip(QApplication::translate("MakeConfig", "\320\237\320\265\321\200\320\265\321\205\320\276\320\264 \320\262 \320\275\320\276\320\262\320\276\320\265 \320\277\320\276\320\273\320\276\320\266\320\265\320\275\320\270\320\265 \321\201\320\276 \321\201\320\261\321\200\320\276\321\201\320\276\320\274 \321\202\320\265\320\272\321\203\321\211\320\265\320\263\320\276 \320\277\320\276\320\273\320\276\320\266\320\265\320\275\320\270\321\217 \320\262 \320\275\321\203\320\273\321\214", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chkRestart->setText(QApplication::translate("MakeConfig", "\321\201 \320\275\321\203\320\273\321\217", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnGo->setToolTip(QApplication::translate("MakeConfig", "\320\277\320\276\320\265\321\205\320\260\320\273\320\270 \320\262 \321\202\320\276\321\207\320\272\321\203 (X,Y)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btnGo->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnUp->setToolTip(QApplication::translate("MakeConfig", "\320\277\320\276\320\264\320\275\321\217\321\202\321\214 \320\264\320\276\320\267\320\260\321\202\320\276\321\200", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btnUp->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnDown->setToolTip(QApplication::translate("MakeConfig", "\320\276\320\277\321\203\321\201\321\202\320\270\321\202\321\214 \320\264\320\276\320\267\320\260\321\202\320\276\321\200", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btnDown->setText(QString());
        label_5->setText(QApplication::translate("MakeConfig", "\320\274\320\273", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnPipi->setToolTip(QApplication::translate("MakeConfig", "\320\222\321\213\320\273\320\270\321\202\321\214 \320\262 (\320\245,\320\243)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btnPipi->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnTryAll->setToolTip(QApplication::translate("MakeConfig", "\321\202\320\265\321\201\321\202\320\276\320\262\321\213\320\271 \320\277\320\276\320\273\320\270\320\262", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btnTryAll->setText(QString());
        btnCmd->setText(QApplication::translate("MakeConfig", "\320\222\320\262\320\276\320\264", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("MakeConfig", "\320\244\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
        mnuTable->setTitle(QApplication::translate("MakeConfig", "\320\242\320\260\320\261\320\273\320\270\321\206\320\260", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("MakeConfig", "\320\232\320\276\320\274\320\260\320\275\320\264\321\213", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MakeConfig: public Ui_MakeConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAKECONFIG_H
