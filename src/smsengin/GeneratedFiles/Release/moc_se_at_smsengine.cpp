/****************************************************************************
** Meta object code from reading C++ file 'se_at_smsengine.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../se_at_smsengine.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'se_at_smsengine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_se__CATSmsEngine_t {
    QByteArrayData data[8];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_se__CATSmsEngine_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_se__CATSmsEngine_t qt_meta_stringdata_se__CATSmsEngine = {
    {
QT_MOC_LITERAL(0, 0, 16), // "se::CATSmsEngine"
QT_MOC_LITERAL(1, 17, 9), // "onATEvent"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 25), // "se::t_ATResponceSharedPtr"
QT_MOC_LITERAL(4, 54, 6), // "pEvent"
QT_MOC_LITERAL(5, 61, 14), // "onTaskFinished"
QT_MOC_LITERAL(6, 76, 11), // "STaskResult"
QT_MOC_LITERAL(7, 88, 11) // "strResponce"

    },
    "se::CATSmsEngine\0onATEvent\0\0"
    "se::t_ATResponceSharedPtr\0pEvent\0"
    "onTaskFinished\0STaskResult\0strResponce"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_se__CATSmsEngine[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x09 /* Protected */,
       5,    1,   27,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,

       0        // eod
};

void se::CATSmsEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CATSmsEngine *_t = static_cast<CATSmsEngine *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onATEvent((*reinterpret_cast< se::t_ATResponceSharedPtr(*)>(_a[1]))); break;
        case 1: _t->onTaskFinished((*reinterpret_cast< const STaskResult(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject se::CATSmsEngine::staticMetaObject = {
    { &IATSmsEngine::staticMetaObject, qt_meta_stringdata_se__CATSmsEngine.data,
      qt_meta_data_se__CATSmsEngine,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *se::CATSmsEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *se::CATSmsEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_se__CATSmsEngine.stringdata0))
        return static_cast<void*>(const_cast< CATSmsEngine*>(this));
    return IATSmsEngine::qt_metacast(_clname);
}

int se::CATSmsEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IATSmsEngine::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
