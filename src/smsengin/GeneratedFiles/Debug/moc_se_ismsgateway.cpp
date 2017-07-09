/****************************************************************************
** Meta object code from reading C++ file 'se_ismsgateway.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../se_ismsgateway.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'se_ismsgateway.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_se__ISmsGetway_t {
    QByteArrayData data[5];
    char stringdata0[43];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_se__ISmsGetway_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_se__ISmsGetway_t qt_meta_stringdata_se__ISmsGetway = {
    {
QT_MOC_LITERAL(0, 0, 14), // "se::ISmsGetway"
QT_MOC_LITERAL(1, 15, 8), // "sigError"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 10), // "se::CError"
QT_MOC_LITERAL(4, 36, 6) // "oError"

    },
    "se::ISmsGetway\0sigError\0\0se::CError\0"
    "oError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_se__ISmsGetway[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void se::ISmsGetway::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ISmsGetway *_t = static_cast<ISmsGetway *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigError((*reinterpret_cast< const se::CError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ISmsGetway::*_t)(se::CError const & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ISmsGetway::sigError)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject se::ISmsGetway::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_se__ISmsGetway.data,
      qt_meta_data_se__ISmsGetway,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *se::ISmsGetway::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *se::ISmsGetway::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_se__ISmsGetway.stringdata0))
        return static_cast<void*>(const_cast< ISmsGetway*>(this));
    return QObject::qt_metacast(_clname);
}

int se::ISmsGetway::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void se::ISmsGetway::sigError(se::CError const & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_se__IATDevice_t {
    QByteArrayData data[9];
    char stringdata0[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_se__IATDevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_se__IATDevice_t qt_meta_stringdata_se__IATDevice = {
    {
QT_MOC_LITERAL(0, 0, 13), // "se::IATDevice"
QT_MOC_LITERAL(1, 14, 16), // "sigResponseReady"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 18), // "se::t_ATCommandPtr"
QT_MOC_LITERAL(4, 51, 10), // "pATCommand"
QT_MOC_LITERAL(5, 62, 10), // "sigATEvent"
QT_MOC_LITERAL(6, 73, 25), // "se::t_ATResponceSharedPtr"
QT_MOC_LITERAL(7, 99, 6), // "pEvent"
QT_MOC_LITERAL(8, 106, 5) // "aData"

    },
    "se::IATDevice\0sigResponseReady\0\0"
    "se::t_ATCommandPtr\0pATCommand\0sigATEvent\0"
    "se::t_ATResponceSharedPtr\0pEvent\0aData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_se__IATDevice[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       5,    1,   32,    2, 0x06 /* Public */,
       5,    1,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::QByteArray,    8,

       0        // eod
};

void se::IATDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IATDevice *_t = static_cast<IATDevice *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigResponseReady((*reinterpret_cast< se::t_ATCommandPtr(*)>(_a[1]))); break;
        case 1: _t->sigATEvent((*reinterpret_cast< se::t_ATResponceSharedPtr(*)>(_a[1]))); break;
        case 2: _t->sigATEvent((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< se::t_ATCommandPtr >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IATDevice::*_t)(se::t_ATCommandPtr );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IATDevice::sigResponseReady)) {
                *result = 0;
            }
        }
        {
            typedef void (IATDevice::*_t)(se::t_ATResponceSharedPtr );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IATDevice::sigATEvent)) {
                *result = 1;
            }
        }
        {
            typedef void (IATDevice::*_t)(QByteArray const & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IATDevice::sigATEvent)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject se::IATDevice::staticMetaObject = {
    { &ISmsGetway::staticMetaObject, qt_meta_stringdata_se__IATDevice.data,
      qt_meta_data_se__IATDevice,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *se::IATDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *se::IATDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_se__IATDevice.stringdata0))
        return static_cast<void*>(const_cast< IATDevice*>(this));
    return ISmsGetway::qt_metacast(_clname);
}

int se::IATDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ISmsGetway::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void se::IATDevice::sigResponseReady(se::t_ATCommandPtr _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void se::IATDevice::sigATEvent(se::t_ATResponceSharedPtr _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void se::IATDevice::sigATEvent(QByteArray const & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
