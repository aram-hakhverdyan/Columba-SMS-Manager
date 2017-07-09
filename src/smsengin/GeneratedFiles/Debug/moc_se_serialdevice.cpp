/****************************************************************************
** Meta object code from reading C++ file 'se_serialdevice.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../se_serialdevice.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'se_serialdevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_se__CSerialDevice_t {
    QByteArrayData data[9];
    char stringdata0[110];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_se__CSerialDevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_se__CSerialDevice_t qt_meta_stringdata_se__CSerialDevice = {
    {
QT_MOC_LITERAL(0, 0, 17), // "se::CSerialDevice"
QT_MOC_LITERAL(1, 18, 14), // "sigRssiChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 9), // "nNewValue"
QT_MOC_LITERAL(4, 44, 9), // "nOldValue"
QT_MOC_LITERAL(5, 54, 11), // "onReadyRead"
QT_MOC_LITERAL(6, 66, 7), // "onError"
QT_MOC_LITERAL(7, 74, 28), // "QSerialPort::SerialPortError"
QT_MOC_LITERAL(8, 103, 6) // "eError"

    },
    "se::CSerialDevice\0sigRssiChanged\0\0"
    "nNewValue\0nOldValue\0onReadyRead\0onError\0"
    "QSerialPort::SerialPortError\0eError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_se__CSerialDevice[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   34,    2, 0x09 /* Protected */,
       6,    1,   35,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void se::CSerialDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CSerialDevice *_t = static_cast<CSerialDevice *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigRssiChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->onReadyRead(); break;
        case 2: _t->onError((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CSerialDevice::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CSerialDevice::sigRssiChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject se::CSerialDevice::staticMetaObject = {
    { &IATDevice::staticMetaObject, qt_meta_stringdata_se__CSerialDevice.data,
      qt_meta_data_se__CSerialDevice,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *se::CSerialDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *se::CSerialDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_se__CSerialDevice.stringdata0))
        return static_cast<void*>(const_cast< CSerialDevice*>(this));
    return IATDevice::qt_metacast(_clname);
}

int se::CSerialDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IATDevice::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void se::CSerialDevice::sigRssiChanged(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
