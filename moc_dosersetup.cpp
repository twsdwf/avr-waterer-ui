/****************************************************************************
** Meta object code from reading C++ file 'dosersetup.h'
**
** Created: Wed Sep 23 20:48:27 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dosersetup.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dosersetup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_dosersetup[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      39,   11,   11,   11, 0x08,
      71,   62,   11,   11, 0x08,
     105,   99,   11,   11, 0x08,
     134,   11,   11,   11, 0x08,
     155,   11,   11,   11, 0x08,
     184,  176,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_dosersetup[] = {
    "dosersetup\0\0on_btnDeleteLast_clicked()\0"
    "on_btSavePos_clicked()\0position\0"
    "on_slAngle_sliderMoved(int)\0value\0"
    "on_slAngle_valueChanged(int)\0"
    "on_btnTest_clicked()\0on_btnMove_clicked()\0"
    "checked\0on_rdMultiDosers_toggled(bool)\0"
};

void dosersetup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        dosersetup *_t = static_cast<dosersetup *>(_o);
        switch (_id) {
        case 0: _t->on_btnDeleteLast_clicked(); break;
        case 1: _t->on_btSavePos_clicked(); break;
        case 2: _t->on_slAngle_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_slAngle_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_btnTest_clicked(); break;
        case 5: _t->on_btnMove_clicked(); break;
        case 6: _t->on_rdMultiDosers_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData dosersetup::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject dosersetup::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_dosersetup,
      qt_meta_data_dosersetup, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &dosersetup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *dosersetup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *dosersetup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_dosersetup))
        return static_cast<void*>(const_cast< dosersetup*>(this));
    return QDialog::qt_metacast(_clname);
}

int dosersetup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
