/****************************************************************************
** Meta object code from reading C++ file 'carstablemodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "operations/carstablemodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'carstablemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Operations__CarsTableModel_t {
    QByteArrayData data[10];
    char stringdata[122];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Operations__CarsTableModel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Operations__CarsTableModel_t qt_meta_stringdata_Operations__CarsTableModel = {
    {
QT_MOC_LITERAL(0, 0, 26), // "Operations::CarsTableModel"
QT_MOC_LITERAL(1, 27, 14), // "propertyChange"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 20), // "PropertyChangeEvent*"
QT_MOC_LITERAL(4, 64, 1), // "e"
QT_MOC_LITERAL(5, 66, 21), // "setColumnToHoldButton"
QT_MOC_LITERAL(6, 88, 7), // "JTable*"
QT_MOC_LITERAL(7, 96, 5), // "table"
QT_MOC_LITERAL(8, 102, 6), // "column"
QT_MOC_LITERAL(9, 109, 12) // "QPushButton*"

    },
    "Operations::CarsTableModel\0propertyChange\0"
    "\0PropertyChangeEvent*\0e\0setColumnToHoldButton\0"
    "JTable*\0table\0column\0QPushButton*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Operations__CarsTableModel[] = {

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
       1,    1,   24,    2, 0x0a /* Public */,
       5,    3,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Int, 0x80000000 | 9,    7,    8,    2,

       0        // eod
};

void Operations::CarsTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CarsTableModel *_t = static_cast<CarsTableModel *>(_o);
        switch (_id) {
        case 0: _t->propertyChange((*reinterpret_cast< PropertyChangeEvent*(*)>(_a[1]))); break;
        case 1: _t->setColumnToHoldButton((*reinterpret_cast< JTable*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QPushButton*(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject Operations::CarsTableModel::staticMetaObject = {
    { &AbstractTableModel::staticMetaObject, qt_meta_stringdata_Operations__CarsTableModel.data,
      qt_meta_data_Operations__CarsTableModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Operations::CarsTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Operations::CarsTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Operations__CarsTableModel.stringdata))
        return static_cast<void*>(const_cast< CarsTableModel*>(this));
    return AbstractTableModel::qt_metacast(_clname);
}

int Operations::CarsTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractTableModel::qt_metacall(_c, _id, _a);
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
