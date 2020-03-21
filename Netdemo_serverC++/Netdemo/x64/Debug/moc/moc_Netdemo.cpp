/****************************************************************************
** Meta object code from reading C++ file 'Netdemo.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Netdemo.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Netdemo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Netdemo_t {
    QByteArrayData data[11];
    char stringdata0[110];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Netdemo_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Netdemo_t qt_meta_stringdata_Netdemo = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Netdemo"
QT_MOC_LITERAL(1, 8, 16), // "RealtimeDataSlot"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 10), // "start_deal"
QT_MOC_LITERAL(4, 37, 9), // "send_deal"
QT_MOC_LITERAL(5, 47, 9), // "stop_deal"
QT_MOC_LITERAL(6, 57, 9), // "copy_deal"
QT_MOC_LITERAL(7, 67, 10), // "clear_deal"
QT_MOC_LITERAL(8, 78, 10), // "stop2_deal"
QT_MOC_LITERAL(9, 89, 10), // "copy2_deal"
QT_MOC_LITERAL(10, 100, 9) // "Cal2_deal"

    },
    "Netdemo\0RealtimeDataSlot\0\0start_deal\0"
    "send_deal\0stop_deal\0copy_deal\0clear_deal\0"
    "stop2_deal\0copy2_deal\0Cal2_deal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Netdemo[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    0,   66,    2, 0x08 /* Private */,
      10,    0,   67,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
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

void Netdemo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Netdemo *_t = static_cast<Netdemo *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->RealtimeDataSlot(); break;
        case 1: _t->start_deal(); break;
        case 2: _t->send_deal(); break;
        case 3: _t->stop_deal(); break;
        case 4: _t->copy_deal(); break;
        case 5: _t->clear_deal(); break;
        case 6: _t->stop2_deal(); break;
        case 7: _t->copy2_deal(); break;
        case 8: _t->Cal2_deal(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Netdemo::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Netdemo.data,
      qt_meta_data_Netdemo,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Netdemo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Netdemo::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Netdemo.stringdata0))
        return static_cast<void*>(const_cast< Netdemo*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Netdemo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
