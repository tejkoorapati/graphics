/****************************************************************************
** Meta object code from reading C++ file 'AppWindow.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AppWindow.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AppWindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AppWindow_t {
    QByteArrayData data[10];
    char stringdata[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AppWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AppWindow_t qt_meta_stringdata_AppWindow = {
    {
QT_MOC_LITERAL(0, 0, 9), // "AppWindow"
QT_MOC_LITERAL(1, 10, 8), // "new_game"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 5), // "reset"
QT_MOC_LITERAL(4, 26, 10), // "wire_frame"
QT_MOC_LITERAL(5, 37, 4), // "face"
QT_MOC_LITERAL(6, 42, 14), // "multi_coloured"
QT_MOC_LITERAL(7, 57, 9), // "slowSpeed"
QT_MOC_LITERAL(8, 67, 11), // "normalSpeed"
QT_MOC_LITERAL(9, 79, 9) // "fastSpeed"

    },
    "AppWindow\0new_game\0\0reset\0wire_frame\0"
    "face\0multi_coloured\0slowSpeed\0normalSpeed\0"
    "fastSpeed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AppWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AppWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AppWindow *_t = static_cast<AppWindow *>(_o);
        switch (_id) {
        case 0: _t->new_game(); break;
        case 1: _t->reset(); break;
        case 2: _t->wire_frame(); break;
        case 3: _t->face(); break;
        case 4: _t->multi_coloured(); break;
        case 5: _t->slowSpeed(); break;
        case 6: _t->normalSpeed(); break;
        case 7: _t->fastSpeed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject AppWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_AppWindow.data,
      qt_meta_data_AppWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AppWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AppWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AppWindow.stringdata))
        return static_cast<void*>(const_cast< AppWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int AppWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
