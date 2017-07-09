/****************************************************************************
** Meta object code from reading C++ file 'pb_profilephotoviewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../pb_profilephotoviewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pb_profilephotoviewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_pb__CPhotoButton_t {
    QByteArrayData data[1];
    char stringdata0[17];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_pb__CPhotoButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_pb__CPhotoButton_t qt_meta_stringdata_pb__CPhotoButton = {
    {
QT_MOC_LITERAL(0, 0, 16) // "pb::CPhotoButton"

    },
    "pb::CPhotoButton"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_pb__CPhotoButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void pb::CPhotoButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject pb::CPhotoButton::staticMetaObject = {
    { &CAnimatedButton::staticMetaObject, qt_meta_stringdata_pb__CPhotoButton.data,
      qt_meta_data_pb__CPhotoButton,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *pb::CPhotoButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *pb::CPhotoButton::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_pb__CPhotoButton.stringdata0))
        return static_cast<void*>(const_cast< CPhotoButton*>(this));
    return CAnimatedButton::qt_metacast(_clname);
}

int pb::CPhotoButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CAnimatedButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_pb__CProfilePhotoViewer_t {
    QByteArrayData data[3];
    char stringdata0[47];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_pb__CProfilePhotoViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_pb__CProfilePhotoViewer_t qt_meta_stringdata_pb__CProfilePhotoViewer = {
    {
QT_MOC_LITERAL(0, 0, 23), // "pb::CProfilePhotoViewer"
QT_MOC_LITERAL(1, 24, 21), // "sigSelectPhotoClicked"
QT_MOC_LITERAL(2, 46, 0) // ""

    },
    "pb::CProfilePhotoViewer\0sigSelectPhotoClicked\0"
    ""
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_pb__CProfilePhotoViewer[] = {

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
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void pb::CProfilePhotoViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CProfilePhotoViewer *_t = static_cast<CProfilePhotoViewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigSelectPhotoClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CProfilePhotoViewer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CProfilePhotoViewer::sigSelectPhotoClicked)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject pb::CProfilePhotoViewer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_pb__CProfilePhotoViewer.data,
      qt_meta_data_pb__CProfilePhotoViewer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *pb::CProfilePhotoViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *pb::CProfilePhotoViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_pb__CProfilePhotoViewer.stringdata0))
        return static_cast<void*>(const_cast< CProfilePhotoViewer*>(this));
    return QWidget::qt_metacast(_clname);
}

int pb::CProfilePhotoViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void pb::CProfilePhotoViewer::sigSelectPhotoClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
