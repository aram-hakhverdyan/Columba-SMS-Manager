/****************************************************************************
** Meta object code from reading C++ file 'db_phonebookdatastore.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../db_phonebookdatastore.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'db_phonebookdatastore.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_db__CPhoneBookDataStore_t {
    QByteArrayData data[10];
    char stringdata0[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_db__CPhoneBookDataStore_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_db__CPhoneBookDataStore_t qt_meta_stringdata_db__CPhoneBookDataStore = {
    {
QT_MOC_LITERAL(0, 0, 23), // "db::CPhoneBookDataStore"
QT_MOC_LITERAL(1, 24, 11), // "sigReloaded"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 19), // "sigAboutToBeCleared"
QT_MOC_LITERAL(4, 57, 15), // "sigContactAdded"
QT_MOC_LITERAL(5, 73, 12), // "pb::CContact"
QT_MOC_LITERAL(6, 86, 8), // "oContact"
QT_MOC_LITERAL(7, 95, 26), // "sigContactAboutToBeRemoved"
QT_MOC_LITERAL(8, 122, 11), // "t_ContactID"
QT_MOC_LITERAL(9, 134, 10) // "nContactID"

    },
    "db::CPhoneBookDataStore\0sigReloaded\0"
    "\0sigAboutToBeCleared\0sigContactAdded\0"
    "pb::CContact\0oContact\0sigContactAboutToBeRemoved\0"
    "t_ContactID\0nContactID"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_db__CPhoneBookDataStore[] = {

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
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,
       4,    1,   36,    2, 0x06 /* Public */,
       7,    1,   39,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 8,    9,

       0        // eod
};

void db::CPhoneBookDataStore::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CPhoneBookDataStore *_t = static_cast<CPhoneBookDataStore *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigReloaded(); break;
        case 1: _t->sigAboutToBeCleared(); break;
        case 2: _t->sigContactAdded((*reinterpret_cast< const pb::CContact(*)>(_a[1]))); break;
        case 3: _t->sigContactAboutToBeRemoved((*reinterpret_cast< t_ContactID(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CPhoneBookDataStore::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CPhoneBookDataStore::sigReloaded)) {
                *result = 0;
            }
        }
        {
            typedef void (CPhoneBookDataStore::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CPhoneBookDataStore::sigAboutToBeCleared)) {
                *result = 1;
            }
        }
        {
            typedef void (CPhoneBookDataStore::*_t)(pb::CContact const & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CPhoneBookDataStore::sigContactAdded)) {
                *result = 2;
            }
        }
        {
            typedef void (CPhoneBookDataStore::*_t)(t_ContactID );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CPhoneBookDataStore::sigContactAboutToBeRemoved)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject db::CPhoneBookDataStore::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_db__CPhoneBookDataStore.data,
      qt_meta_data_db__CPhoneBookDataStore,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *db::CPhoneBookDataStore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *db::CPhoneBookDataStore::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_db__CPhoneBookDataStore.stringdata0))
        return static_cast<void*>(const_cast< CPhoneBookDataStore*>(this));
    if (!strcmp(_clname, "IDBComponent"))
        return static_cast< IDBComponent*>(const_cast< CPhoneBookDataStore*>(this));
    return QObject::qt_metacast(_clname);
}

int db::CPhoneBookDataStore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void db::CPhoneBookDataStore::sigReloaded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void db::CPhoneBookDataStore::sigAboutToBeCleared()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void db::CPhoneBookDataStore::sigContactAdded(pb::CContact const & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void db::CPhoneBookDataStore::sigContactAboutToBeRemoved(t_ContactID _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
