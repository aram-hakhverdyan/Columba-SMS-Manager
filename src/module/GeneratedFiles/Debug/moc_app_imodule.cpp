/****************************************************************************
** Meta object code from reading C++ file 'app_imodule.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../app_imodule.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'app_imodule.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_app__IModule_t {
    QByteArrayData data[9];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_app__IModule_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_app__IModule_t qt_meta_stringdata_app__IModule = {
    {
QT_MOC_LITERAL(0, 0, 12), // "app::IModule"
QT_MOC_LITERAL(1, 13, 9), // "sigNotify"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 6), // "sTitle"
QT_MOC_LITERAL(4, 31, 5), // "sText"
QT_MOC_LITERAL(5, 37, 17), // "ENotificationType"
QT_MOC_LITERAL(6, 55, 5), // "eType"
QT_MOC_LITERAL(7, 61, 15), // "sigReportStatus"
QT_MOC_LITERAL(8, 77, 7) // "sStatus"

    },
    "app::IModule\0sigNotify\0\0sTitle\0sText\0"
    "ENotificationType\0eType\0sigReportStatus\0"
    "sStatus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_app__IModule[] = {

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
       1,    3,   29,    2, 0x06 /* Public */,
       7,    1,   36,    2, 0x06 /* Public */,
       7,    0,   39,    2, 0x26 /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 5,    3,    4,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,

       0        // eod
};

void app::IModule::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IModule *_t = static_cast<IModule *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigNotify((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< ENotificationType(*)>(_a[3]))); break;
        case 1: _t->sigReportStatus((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->sigReportStatus(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IModule::*_t)(QString const & , QString const & , ENotificationType );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IModule::sigNotify)) {
                *result = 0;
            }
        }
        {
            typedef void (IModule::*_t)(QString const & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IModule::sigReportStatus)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject app::IModule::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_app__IModule.data,
      qt_meta_data_app__IModule,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *app::IModule::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *app::IModule::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_app__IModule.stringdata0))
        return static_cast<void*>(const_cast< IModule*>(this));
    return QObject::qt_metacast(_clname);
}

int app::IModule::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void app::IModule::sigNotify(QString const & _t1, QString const & _t2, ENotificationType _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void app::IModule::sigReportStatus(QString const & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
