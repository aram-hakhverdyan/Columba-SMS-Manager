/****************************************************************************
** Meta object code from reading C++ file 'pb_contactheadermodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../pb_contactheadermodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pb_contactheadermodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_pb__CContactHeaderModel_t {
    QByteArrayData data[10];
    char stringdata0[148];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_pb__CContactHeaderModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_pb__CContactHeaderModel_t qt_meta_stringdata_pb__CContactHeaderModel = {
    {
QT_MOC_LITERAL(0, 0, 23), // "pb::CContactHeaderModel"
QT_MOC_LITERAL(1, 24, 12), // "onUpdateData"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 23), // "onStoreAboutToBeCleared"
QT_MOC_LITERAL(4, 62, 14), // "onContactAdded"
QT_MOC_LITERAL(5, 77, 12), // "pb::CContact"
QT_MOC_LITERAL(6, 90, 8), // "oContact"
QT_MOC_LITERAL(7, 99, 25), // "onContactAboutToBeRemoved"
QT_MOC_LITERAL(8, 125, 11), // "t_ContactID"
QT_MOC_LITERAL(9, 137, 10) // "nContactID"

    },
    "pb::CContactHeaderModel\0onUpdateData\0"
    "\0onStoreAboutToBeCleared\0onContactAdded\0"
    "pb::CContact\0oContact\0onContactAboutToBeRemoved\0"
    "t_ContactID\0nContactID"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_pb__CContactHeaderModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    0,   35,    2, 0x09 /* Protected */,
       4,    1,   36,    2, 0x09 /* Protected */,
       7,    1,   39,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 8,    9,

       0        // eod
};

void pb::CContactHeaderModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CContactHeaderModel *_t = static_cast<CContactHeaderModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onUpdateData(); break;
        case 1: _t->onStoreAboutToBeCleared(); break;
        case 2: _t->onContactAdded((*reinterpret_cast< const pb::CContact(*)>(_a[1]))); break;
        case 3: _t->onContactAboutToBeRemoved((*reinterpret_cast< t_ContactID(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject pb::CContactHeaderModel::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_pb__CContactHeaderModel.data,
      qt_meta_data_pb__CContactHeaderModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *pb::CContactHeaderModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *pb::CContactHeaderModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_pb__CContactHeaderModel.stringdata0))
        return static_cast<void*>(const_cast< CContactHeaderModel*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int pb::CContactHeaderModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
