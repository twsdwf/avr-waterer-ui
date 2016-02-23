#-------------------------------------------------
#
# Project created by QtCreator 2013-06-07T15:59:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = watering_analizer
TEMPLATE = app

SOURCES += main.cpp\
    com/qextserialport.cpp \
    com/qextserialenumerator_unix.cpp \
    com/posix_qextserialport.cpp \
    globalstatedialog.cpp \
    waterstorages.cpp \
    visualconfig.cpp \
    connectdlg.cpp

HEADERS  += \
    com/qextserialenumerator.h \
    com/qextserialport_global.h \
    com/qextserialport.h \
    data_structs.h \
    globalstatedialog.h \
    waterstorages.h \
    visualconfig.h \
    connectdlg.h

FORMS    += \
    globalstatedialog.ui \
    waterstorages.ui \
    visualconfig.ui \
    connectdlg.ui

OTHER_FILES +=

RESOURCES += \
    resources.qrc
