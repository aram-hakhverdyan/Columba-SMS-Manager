/****************************************************************************
** Meta object code from reading C++ file 'se_ismsengine.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../se_ismsengine.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'se_ismsengine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_se__ISmsEngine_t {
    QByteArrayData data[11];
    char stringdata0[146];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_se__ISmsEngine_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_se__ISmsEngine_t qt_meta_stringdata_se__ISmsEngine = {
    {
QT_MOC_LITERAL(0, 0, 14), // "se::ISmsEngine"
QT_MOC_LITERAL(1, 15, 13), // "sigNewMessage"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 25), // "se::t_SmsMessageSharedPtr"
QT_MOC_LITERAL(4, 56, 8), // "pMessage"
QT_MOC_LITERAL(5, 65, 17), // "sigMessageWasSent"
QT_MOC_LITERAL(6, 83, 19), // "sigLastTaskFinished"
QT_MOC_LITERAL(7, 103, 15), // "se::STaskResult"
QT_MOC_LITERAL(8, 119, 8), // "sigError"
QT_MOC_LITERAL(9, 128, 10), // "se::CError"
QT_MOC_LITERAL(10, 139, 6) // "oError"

    },
    "se::ISmsEngine\0sigNewMessage\0\0"
    "se::t_SmsMessageSharedPtr\0pMessage\0"
    "sigMessageWasSent\0sigLastTaskFinished\0"
    "se::STaskResult\0sigError\0se::CError\0"
    "oError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_se__ISmsEngine[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    1,   37,    2, 0x06 /* Public */,
       6,    1,   40,    2, 0x06 /* Public */,
       8,    1,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void se::ISmsEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ISmsEngine *_t = static_cast<ISmsEngine *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigNewMessage((*reinterpret_cast< se::t_SmsMessageSharedPtr(*)>(_a[1]))); break;
        case 1: _t->sigMessageWasSent((*reinterpret_cast< se::t_SmsMessageSharedPtr(*)>(_a[1]))); break;
        case 2: _t->sigLastTaskFinished((*reinterpret_cast< se::STaskResult(*)>(_a[1]))); break;
        case 3: _t->sigError((*reinterpret_cast< const se::CError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ISmsEngine::*_t)(se::t_SmsMessageSharedPtr );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ISmsEngine::sigNewMessage)) {
                *result = 0;
            }
        }
        {
            typedef void (ISmsEngine::*_t)(se::t_SmsMessageSharedPtr );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ISmsEngine::sigMessageWasSent)) {
                *result = 1;
            }
        }
        {
            typedef void (ISmsEngine::*_t)(se::STaskResult );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ISmsEngine::sigLastTaskFinished)) {
                *result = 2;
            }
        }
        {
            typedef void (ISmsEngine::*_t)(se::CError const & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ISmsEngine::sigError)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject se::ISmsEngine::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_se__ISmsEngine.data,
      qt_meta_data_se__ISmsEngine,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *se::ISmsEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *se::ISmsEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_se__ISmsEngine.stringdata0))
        return static_cast<void*>(const_cast< ISmsEngine*>(this));
    return QObject::qt_metacast(_clname);
}

int se::ISmsEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void se::ISmsEngine::sigNewMessage(se::t_SmsMessageSharedPtr _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void se::ISmsEngine::sigMessageWasSent(se::t_SmsMessageSharedPtr _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void se::ISmsEngine::sigLastTaskFinished(se::STaskResult _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void se::ISmsEngine::sigError(se::CError const & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
struct qt_meta_stringdata_se__IATSmsEngine_t {
    QByteArrayData data[4];
    char stringdata0[43];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_se__IATSmsEngine_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_se__IATSmsEngine_t qt_meta_stringdata_se__IATSmsEngine = {
    {
QT_MOC_LITERAL(0, 0, 16), // "se::IATSmsEngine"
QT_MOC_LITERAL(1, 17, 14), // "sigRssiChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 9) // "nNewValue"

    },
    "se::IATSmsEngine\0sigRssiChanged\0\0"
    "nNewValue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_se__IATSmsEngine[] = {

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
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void se::IATSmsEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IATSmsEngine *_t = static_cast<IATSmsEngine *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigRssiChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IATSmsEngine::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IATSmsEngine::sigRssiChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject se::IATSmsEngine::staticMetaObject = {
    { &ISmsEngine::staticMetaObject, qt_meta_stringdata_se__IATSmsEngine.data,
      qt_meta_data_se__IATSmsEngine,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *se::IATSmsEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *se::IATSmsEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_se__IATSmsEngine.stringdata0))
        return static_cast<void*>(const_cast< IATSmsEngine*>(this));
    return ISmsEngine::qt_metacast(_clname);
}

int se::IATSmsEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ISmsEngine::qt_metacall(_c, _id, _a);
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
void se::IATSmsEngine::sigRssiChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
